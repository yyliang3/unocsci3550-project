#!/usr/bin/perl

use Modern::Perl;


use CGI qw/:standard/;
use CGI::Carp qw/fatalsToBrowser/;

print header(), start_html(
    -style=>{'src'=>'my.css'},
    -title=>'Palindrome/Comments Page'
);

print << "BLOCK";

<div id= "navbar">
<div id="header">       <!-- this is the header for the document -->
    <div id="title">    <!--. obviously the title of the work -->
               CGI- palidrome
        </div>
    
   <div id="nav">      <!-- a short navigation section to jump in doc -->
	<br/> <br/>
	<a href="page5.html"><button> CGI </button></a><br/>

  
  </div>
<div id="assert">
Zexi Web Design <br/> &copy; 2018
                                                                                                                                                     </div>
</div>
</div>
BLOCK

print "<div id=\"content\">";  



# Your code solution goes here!!

my $count = 0;
my $foo   = param('one');				# value of first checkbox
if ( $foo )
{
 	print p("This is automatic response by AI. If you need further helping, please call us hotline between\n
	8:00 a.m and 5:00 p.m. We will collect some basic information about you. Please don't worry, \n
	we won't leak it.");
	$count++;
}
else
{
	print p("Checkbox one was not submitted!");
}

my $gender = param('ma');

if ($gender eq "male")
{
	print p("Hello Mr ");
	$count++;
}
elsif($gender eq "female")
{
	print p("Hello Ms ");
	$count++;
}
elsif($gender eq "othermale")
{
	print p("Hello ");
	$count++;
}
else
{
	print p("There is no gender selection");
}

my $fname = param('fname');

if ( $fname ~~ /^\s*$/ )
{		
	print p("First name is empty!");
}
else 
{
	print p ( "$fname " );
	$count++;
}

my $lname = param('lname');

if ( $lname ~~ /^\s*$/ )
{
	print p ( "Last name is empty!" );
}
else
{
	print p ( "$lname" );
	$count++;
}

my $mail = param('mail');

my $zip = param('zip');
if ($zip =~ /^\d+$/)
{
	print p("The zip is $zip");
}
else
{
	print p("You have invalid zip code");
}
my $state = param('state');


print p("The email address is $mail");


print p("The state is $state");


my $text = param('comments');

if ( $text ~~ /^\s*$/ ) {    
    print a ( { -href=>'javascript:history.back();' },
              "Go back and fill out that form!" );
}
else {
    print p( "Here is what you submitted:" ), p( $text );
	$count++;
}

my @model= param('model');	# could be a list!
$" = ", ";

if ( !@model ) {
    print p ( "No perferences were selected!" );
}
else {
    print p("The perference(s) were @model");
	$count++;

}

my $size = @model;

my $totalcost = 100000*$size;

if ($count == 6)
{
	print p("The total cost of your car will be approximately $totalcost \$");
}
else
{
	print a ( { -href=>'javascript:history.back();' },
              "Since we will calculate the costs on your future vehicle, you may want to go back and fill out some questions!" );
}

print "<br/>";

print "<br/>";
#store each character in the string into a separate variable (a1 - a9)

	

print "<br/>";

print "<br/>";

print "\n\n";























print "</div>";

print end_html();

# this is the end of the class as we know it, I feel fine!

