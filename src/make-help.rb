#!/usr/bin/ruby

require "rexml/document"
require "rexml/xpath"

include REXML

######################################################################

def option_line(options, text)
	o = "  "+options.delete("\n")

	o += " " * (34-o.length)

	text.split.each { |e|
		if o.length+1+e.length > 78
			puts   "\""+o
			o = " "*36
		end
		o += " "+e
	}
	puts "\""+o

	#puts   "\""+o+" "+text.delete("\n")+"\\n\"\n"
end

######################################################################

$input = File.new(ARGV[0])
document = REXML::Document.new($input)

info = XPath.first( document, "//info" ) 

puts "Usage: "+info.attributes['name']+" "+XPath.first( document, "//synopsis").get_text.value.delete("\n")

puts "\"OPTIONS:\""
XPath.each( document, "//optionlist") { |optionlist|
	puts "\"\\n\""
	XPath.each( optionlist, "option") { |el|
		short    = el.attributes['short']
		long     = el.attributes['long']
		argument = el.attributes['argument'].to_s
		text     = el.attributes['text'].to_s
		
		# option
		option = ""
		if text != "" 
			option = text
		else
			if short != nil 
				option = "-"+short
				if long != nil
					option += ", "+"--"+long
				end
			else
				option += "--"+long
			end
			option += " " + argument
		end
		
		# descripion of option
		optiontext = XPath.first(el, "shortdesc").get_text.value

		#option_line(option, optiontext)
		puts "\"  "+option+"\\n\""
	}
}

puts "\"\\n\""
puts "\"Please report bugs to " + "<"+info.attributes['email']+">\\n\""
puts ";"

######################################################################
