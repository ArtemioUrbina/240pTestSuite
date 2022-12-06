-- Sailor VROM (Lua version) by freem
-- This is a modified version by Artemio Urbina for the 240p Test Suite
-- Use of this tool (e.g. using the exported files) is free.
-- License is still undecided, but leaning towards public domain/unlicense/CC0.
--============================================================================--
local verNum = 0.20
local pcmbSeparator = "|"
local sizeWarnString = "(needs padding)"
local errorString = ""

local sampleListTypes = {
	["vasm"]	= { Directive="word",	Format="0x%04X" },
	["tniasm"]	= { Directive="dw",		Format="$%04X" }, -- could also use 0x%04X
	["wla"]		= { Directive=".dw",	Format="$%04X" },
}

--============================================================================--
-- Program startup (banner, basic argument check)
local args = {...}

print("Sailor VROM - Neo-Geo V ROM/.PCM File Builder (Lua version)");
print(string.format("v%.02f by freem",verNum))
print(string.format("Modified by Artemio for the 240p test Suite"))
print("===========================================================");

-- check arguments
if not args or not args[1] then
	print("No arguments found.")
	print("usage: lua svrom.lua (options)")
	print("===========================================================");
	print("Available options:")
	print("    --pcma=path         path/filename of ADPCM-A sample list file")
	print("    --pcmb=path         path/filename of ADPCM-B sample list file")
	print("    --outname=path      path/filename of single/ADPCM-A sound data output")
	print("    --outnameB=path     path/filename of ADPCM-B sound data output")
	print("    --samplelist=path   path/filename of sample list output")
	print("    --samplestart=addr  starting address for sample list output")
	print("    --mode=mode         'cd' or 'cart', without the quotes")
	print("    --slformat=format   'vasm', 'tniasm', or 'wla', all without quotes")
	print("    --vrom=format       'split' (ADPCM-A/B) vrom or 'single' vrom, without the quotes")
	return
end

--============================================================================--
-- parse command line parameters
local pcmaListFile, pcmbListFile	-- adpcm-a and adpcm-b sample list input files
local outSoundFile, outSampleFile	-- sound data and sample list output files
local pcmaListFN, pcmbListFN, outSoundFN, outSampleFN, outSoundFNB-- filenames for the above
local modeType = "cart"
local sampListType = "vasm"
local outSampleStart = 0
local vromType = "single"

local possibleParams = {
	["pcma"] = function(input) pcmaListFN = input end,
	["pcmb"] = function(input) pcmbListFN = input end,
	["outname"] = function(input) outSoundFN = input end,
	["samplelist"] = function(input) outSampleFN = input end,
	["samplestart"] = function(input)
		local _start,_end
		if string.find(input,"0x") then -- hex format 1
			_start,_end = string.find(input,"0x")
			outSampleStart = tonumber(string.sub(input,_end+1,-1),16)
		elseif string.find(input,"$") then -- hex format 2
			_start,_end = string.find(input,"$")
			outSampleStart = tonumber(string.sub(input,2,-1),16)
		elseif tonumber(input) then -- decimal
			outSampleStart = tonumber(input)
		else
			error("Invalid input for samplestart: "..input)
		end
	end,
	["mode"] = function(input)
		input = string.lower(input)
		if input ~= "cart" and input ~="cd" then
			error("Mode option must be 'cart' or 'cd'!")
		end
		modeType = input
	end,
	["slformat"] = function(input)
		local foundFormat = false
		input = string.lower(input)
		for t,f in pairs(sampleListTypes) do
			if not foundFormat then
				if input == t then
					sampListType = t
					foundFormat = true
				end
			end
		end
		if not foundFormat then
			print(string.format("Unknown slformat type '%s', using vasm instead.",input))
		end
	end,
	["vrom"] = function(input)
		input = string.lower(input)
		if input ~= "split" and input ~="single" then
			error("VROM format 'split' or 'single'!")
		end
		vromType = input
	end,
	["outnameB"] = function(input) outSoundFNB = input end,
}

local startDash, endDash, startEquals
for k,v in pairs(args) do
	-- search for "--" at the beginning of a string
	startDash,endDash = string.find(v,"--",1,true)
	if not startDash then
		print(string.format("Unrecognized option '%s'.",v))
		return
	end

	-- look for a "=" inside of it
	startEquals = string.find(v,"=",3,true)
	if not startEquals then
		print(string.format("Did not find equals to assign data in '%s'.",v))
		return
	end

	-- decode command and value
	local command = string.sub(v,endDash+1,startEquals-1)
	local value = string.sub(v,startEquals+1,-1)

	-- look for command in table
	local didCommand = false
	for c,f in pairs(possibleParams) do
		if c == command then
			f(value)
			didCommand = true
		end
	end
	if not didCommand then
		print(string.format("Sailor VROM doesn't recognize the command '%s'.",command))
	end
end

--============================================================================--
-- By this point, the commands are parsed. We should have values in the proper
-- variables if the user decided to actually enter some data. Of course, some
-- parameters are optional, so we also handle the fallback filenames here.

print(string.format("Output Mode Type: %s",modeType))
print(string.format("VROM Format: %s",vromType))

if pcmaListFN then
	print(string.format("ADPCM-A sample list file: '%s'",pcmaListFN))
end

-- if ADPCM-B is attempted to be used on a CD system, we need to ignore it.
if pcmbListFN and modeType == "cd" then
	print("Neo-Geo CD does not support ADPCM-B playback. (Yes, we've tried.)")
	print("Ignoring ADPCM-B samples...")
	pcmbListFN = nil
end

-- if split VROM is attempted to be used on a CD system, we need to ignore it.
if vromType == "split" and modeType == "cd" then
	print("Split ROM not supported/needed in Neo-Geo CD")
	print("Ignoring split command...")
	vromType = "single"
end

if pcmbListFN then
	print(string.format("ADPCM-B sample list file: '%s'",pcmbListFN))
end

-- outSoundFN is not mandatory. (defaults to "output.v" or "output.pcm")
if not outSoundFN then
	outSoundFN = "output."..(modeType=="cd" and "pcm" or "v")
	if vromType == "split" then
		print(string.format("ADPCM-A data output filename omitted, using '%s'.",outSoundFN))
	else
		print(string.format("Sound data output filename omitted, using '%s'.",outSoundFN))
	end
else
	if vromType == "split" then
		print(string.format("ADPCM-A data output: %s",outSoundFN))
	else
		print(string.format("Sound data output: %s",outSoundFN))
	end
end

-- outSoundFNB is not mandatory. (defaults to "output.Bv" or "output.Bpcm")
if vromType == "split" then
	if not outSoundFNB then 
		outSoundFNB = "outputBROM."..(modeType=="cd" and "pcm" or "v")
		print(string.format("ADPCM-B data output filename omitted, using '%s'.",outSoundFNB))
	else
		print(string.format("ADPCM-B Sound data output: %s",outSoundFNB))
	end
end

-- outSampleFN is not mandatory either. (defaults to "samples.inc")
if not outSampleFN then
	outSampleFN = "samples.inc"
	print(string.format("Sample address output filename omitted, using '%s'.",outSampleFN))
else
	print(string.format("Sample address output: %s",outSampleFN))
end

print(string.format("sample list address start: %s",outSampleStart))

print(string.format("sample list type: %s",sampListType))

print(string.format("VROM Type: %s", vromType))

print("")

--============================================================================--
-- Whew. That's a lot of checking. We're still not done yet, though, because if
-- those list files turn out to not exist, then I'm gonna get really mad!

if pcmaListFN then
	pcmaListFile,errorString = io.open(pcmaListFN,"r")
	if not pcmaListFile then
		print(string.format("Error attempting to open ADPCM-A list %s",errorString))
		return
	end
end

--[[ Generic List Parsing Variables ]]--
local tempFile, tempLen, tempData
local padMe = false

--[[ ADPCM-A List Parsing ]]--
local pcmaFiles = {}
local pcmaCount = 1

if pcmaListFN then
	print("")
	print("==[ADPCM-A Input Sample List]==")
	for l in pcmaListFile:lines() do
		-- try loading file
		tempFile,errorString = io.open(l,"rb")
		if not tempFile then
			print(string.format("Error attempting to load ADPCM-A sample %s",l))
			print(errorString)
			return
		end

		-- get file length
		tempLen,errorString = tempFile:seek("end")
		if not tempLen then
			print(string.format("Error attempting to get length of ADPCM-A sample %s",l))
			print(errorString)
			return
		end

		tempFile:seek("set")

		padMe = false
		if tempLen % 256 ~= 0 then
			sizeWarn = sizeWarnString
			padMe = true
		else
			sizeWarn = ""
		end

		tempData = tempFile:read(tempLen)
		tempFile:close()
		print(string.format("(PCMA %03i) %s %s",pcmaCount,l,sizeWarn))

		if tempLen > (1024*1024) then
			print(string.format("WARNING: PCMA sample %03i is too large! (>1MB)",pcmaCount))
		end

		if padMe then
			-- pad the sample with 0x80
			local padBytes = 256-(tempLen%256)

			for i=1,padBytes do
				tempData = tempData .. string.char(128)
			end
			tempLen = tempLen + padBytes

			if tempLen > (1024*1024) then
				print(string.format("WARNING: PCMA sample %03i is too large after padding! (>1MB)",pcmaCount))
			end
		end

		table.insert(pcmaFiles,pcmaCount,{ID=pcmaCount-1,File=l,Length=tempLen,Data=tempData})
		pcmaCount = pcmaCount + 1
	end

	pcmaListFile:close()
end

--============================================================================--
-- Time for ADPCM-B, but only if we have it.

--[[ ADPCM-B List Parsing ]]--
local pcmbFiles = {}
local pcmbCount = 0
local tempRate, tempRealFileName

if pcmbListFN then
	pcmbCount = 1
	print("")
	print("==[ADPCM-B Input Sample List]==")

	-- try opening list file
	pcmbListFile,errorString = io.open(pcmbListFN,"r")
	if not pcmbListFile then
		print(string.format("Error attempting to open ADPCM-B list %s",errorString))
		return
	end

	for l in pcmbListFile:lines() do
		-- look for rate splitter character
		local rateSplitter = string.find(l,pcmbSeparator)
		if not rateSplitter then
			print(string.format("ADPCM-B sample %03i does not have a sample rate defined.",pcmbCount))
			return
		end

		-- get actual filename
		tempRealFileName = string.sub(l,1,rateSplitter-1)

		-- get sample rate
		tempRate = tonumber(string.sub(l,rateSplitter+1))
		if not tempRate then
			print(string.format("Error decoding sample rate from string '%s'",string.sub(l,rateSplitter+1)))
		end
		if tempRate < 1800 or tempRate > 55500 then
			print(string.format("ADPCM-B sample %s has invalid sampling rate %dHz, must be between 1800Hz and 55500Hz",tempRealFileName,tempRate))
			return
		end

		-- try loading file
		tempFile,errorString = io.open(tempRealFileName,"rb")
		if not tempFile then
			print(string.format("Error attempting to load ADPCM-B sample %s",errorString))
			return
		end

		-- get file length
		tempLen,errorString = tempFile:seek("end")
		if not tempLen then
			print(string.format("Error attempting to get length of ADPCM-B sample %s",errorString))
			return
		end

		tempFile:seek("set")

		padMe = false
		if tempLen % 256 ~= 0 then
			sizeWarn = sizeWarnString
			padMe = true
		else
			sizeWarn = ""
		end

		tempData = tempFile:read(tempLen)
		tempFile:close()
		print(string.format("(PCMB %03i) %s (rate %d) %s",pcmbCount,tempRealFileName,tempRate,sizeWarn))

		if padMe then
			-- pad the sample with 0x80
			local padBytes = 256-(tempLen%256)

			for i=1,padBytes do
				tempData = tempData .. string.char(128)
			end
			tempLen = tempLen + padBytes
		end

		table.insert(pcmbFiles,pcmbCount,{ID=pcmbCount-1,File=tempRealFileName,Length=tempLen,Rate=tempRate,Data=tempData})
		pcmbCount = pcmbCount + 1
	end

	pcmbListFile:close()
end

print("")

--============================================================================--
-- Determine sample bank layouts (a.k.a. try to avoid crossing 1MB boundaries)
local curBankSize = 0
local curBankNum = 1
local curIndex = 1

local banksA = {}
local banksB = {}

-- handle ADPCM-A first
if pcmaListFN then
	for k,v in pairs(pcmaFiles) do
		if not banksA[curBankNum] then table.insert(banksA,curBankNum,{}) end

		-- check if adding current file to bank would cause overflow
		if (v.Length + curBankSize) > 1024*1024 then
			print("------------------------------------------------")
			print(string.format("PCMA bank %d: add %s (overflow from bank %d)",curBankNum+1,v.File,curBankNum))
			-- needs to go to next bank
			curBankNum = curBankNum + 1
			curBankSize = v.Length
			curIndex = 1
			if not banksA[curBankNum] then table.insert(banksA,curBankNum,{}) end
			table.insert(banksA[curBankNum],curIndex,v)
			curIndex = curIndex + 1
		else
			-- add to current bank
			print(string.format("PCMA bank %d: add %s",curBankNum,v.File))
			curBankSize = curBankSize + v.Length
			table.insert(banksA[curBankNum],curIndex,v)
			curIndex = curIndex + 1
		end
	end
end

-- handle ADPCM-B next (if applicable)
if pcmbListFN then
	curBankSize = 0
	curIndex = 1
	curBankNum = 1

	for k,v in pairs(pcmbFiles) do
		if not banksB[curBankNum] then table.insert(banksB,curBankNum,{}) end

		-- ADPCM-B has no restrictions on sample sizes, according to datasheet

		-- add to current bank
		print(string.format("PCMB bank %d: add %s",curBankNum,v.File))
		curBankSize = curBankSize + v.Length
		table.insert(banksB[curBankNum],curIndex,v)
		curIndex = curIndex + 1
	end
end

print("")

--============================================================================--
-- Read bank layouts and figure out required padding.
local paddingA = {}
local ROMSize = 0;

if pcmaListFN then
	print("Reading ADPCM-A bank layouts...")
	curBankSize = 0

	for k,v in pairs(banksA) do
		for i,d in pairs(v) do
			curBankSize = curBankSize + d.Length
		end
		print(string.format("size of ADPCM-A bank %d: %d bytes",k,curBankSize))
		if curBankSize < 1024*1024 and k ~= #banksA then
			local diff = (1024*1024) - curBankSize
			print(string.format("Padding required: %d bytes",diff))
			table.insert(paddingA,k,diff)
		end
		print("------------------------------------------------")
		ROMSize = curBankSize;
		curBankSize = 0
	end
end

if pcmbListFN then
	print("Reading ADPCM-B bank layouts...")
	curBankSize = 0

	for k,v in pairs(banksB) do
		for i,d in pairs(v) do
			curBankSize = curBankSize + d.Length
		end
		print(string.format("size of ADPCM-B bank %d: %d bytes",k,curBankSize))
		print("------------------------------------------------")
		ROMSize = ROMSize + curBankSize;
		curBankSize = 0
	end
end

if vromType == "single" then
	print(string.format("Total ROM Size %d bytes", ROMSize))
end

print("")

--============================================================================--
-- Create the combined sample rom using bank layout

print("Creating combined sample data...")

outSoundFile,errorString = io.open(outSoundFN,"w+b")
if not outSoundFile then
	print(string.format("Error attempting to create output file %s",errorString))
	return
end

-- ADPCM-A
if pcmaListFN then
	for k,v in pairs(banksA) do
		-- write sounds in this bank
		print(string.format("Writing ADPCM-A bank %d...",k))
		for i,d in pairs(v) do
			outSoundFile:write(d.Data)
		end

		if paddingA[k] and k ~= #banksA then
			print(string.format("Writing %d bytes of padding...",paddingA[k]))
			-- write padding for this bank
			for i=1,paddingA[k] do
				outSoundFile:write(string.char(128))
			end
		end
		print("------------------------------------------------")
	end
end

if vromType == "split" then
	outSoundFile:close()

	print("Resetting offset and switching file for ADPCM-B...")
	outSoundFile,errorString = io.open(outSoundFNB,"w+b")
	if not outSoundFile then
		print(string.format("Error attempting to create output file %s",errorString))
		return
	end
end

if pcmbListFN then
	for k,v in pairs(banksB) do
		-- write sounds in this bank
		print("Writing ADPCM-B bank...")
		for i,d in pairs(v) do
			outSoundFile:write(d.Data)
		end
		print("------------------------------------------------")
	end
end

outSoundFile:close()

print("")

--============================================================================--
-- Get sample addresses (using banks to account for padding)

local sampleStart = 0
if outSampleStart then
	sampleStart = outSampleStart
end

if pcmaListFN then
	print("Calculating ADPCM-A sample addresses...")

	-- ADPCM-A samples
	for k,v in pairs(banksA) do
		for i,d in pairs(v) do
			local fixedLen = (d.Length/256)
			d.Start = sampleStart
			d.End = (sampleStart+fixedLen)-1
			sampleStart = sampleStart + fixedLen
		end
		if paddingA[k] and k ~= #banksA then
			sampleStart = sampleStart + (paddingA[k]/256)
		end
	end
end

-- ADPCM-B samples
if pcmbListFN then
	print("Calculating ADPCM-B sample addresses...")

	if vromType == "split" then
		sampleStart = 0
	end
	for k,v in pairs(banksB) do
		for i,d in pairs(v) do
			local fixedLen = (d.Length/256)
			d.Start = sampleStart
			d.End = (sampleStart+fixedLen)-1
			d.DeltaN = (d.Rate/55500)*65536
			sampleStart = sampleStart + fixedLen
		end
	end
end

print("")

--============================================================================--
-- create the sample address list

print("Creating sample address list...")

outSampleFile,errorString = io.open(outSampleFN,"w+")
if not outSampleFile then
	print(string.format("Error attempting to create sample list file %s",errorString))
	return
end

local direc = sampleListTypes[sampListType].Directive
local valFormat = sampleListTypes[sampListType].Format

-- write header
outSampleFile:write("; Sample address list generated by Sailor VROM\n")
outSampleFile:write(";==============================================;\n")
outSampleFile:write("\n")

-- write ADPCM-A
if pcmaListFN then
	outSampleFile:write("; [ADPCM-A Samples]\n")
	outSampleFile:write("samples_PCMA:\n")

	for k,v in pairs(pcmaFiles) do
		outSampleFile:write(string.format("\t%s\t"..valFormat..","..valFormat.."\t; PCMA Sample #%i (%s)\n",direc,v.Start,v.End,v.ID,v.File))
	end
	outSampleFile:write("\n")
end

-- write ADPCM-B, if applicable
if pcmbListFN then
	outSampleFile:write("; [ADPCM-B Samples]\n")
	outSampleFile:write("samples_PCMB:\n")

	for k,v in pairs(pcmbFiles) do
		outSampleFile:write(string.format("\t%s\t"..valFormat..","..valFormat.."\t; PCMB Sample #%i (%s, %dHz)\n",direc,v.Start,v.End,v.ID,v.File,v.Rate))
	end
	outSampleFile:write("\n")

	outSampleFile:write("; [ADPCM-B Default Sample Rates]\n")
	outSampleFile:write("rates_PCMB:\n")
	for k,v in pairs(pcmbFiles) do
		outSampleFile:write(string.format("\t%s\t"..valFormat.."\t; PCMB Sample #%i (%s, %dHz)\n",direc,math.floor(v.DeltaN),v.ID,v.File,v.Rate))
	end
	outSampleFile:write("\n")
end

outSampleFile:close()

--============================================================================--
print("")
print("Build successful.")
