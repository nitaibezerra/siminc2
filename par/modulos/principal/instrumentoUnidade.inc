<?php 
//include_once APPRAIZ . "includes/workflow.php";
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

//echo montarAbasArray( criaAbaEscolaAtiva(true), "escolaativa.php?modulo=principal/estruturaAvaliacao&acao=A" );
						  						  
monta_titulo( $titulo_modulo, '' );
?>
<link rel="stylesheet" href="../includes/jquery-treeview/jquery.treeview.css" />
<link rel="stylesheet" href="../includes/jquery-treeview/red-treeview.css" />
<link rel="stylesheet" href="screen.css" />

<script src="../includes/jquery-treeview/lib/jquery.js" type="text/javascript"></script>
<script src="../includes/jquery-treeview/lib/jquery.cookie.js" type="text/javascript"></script>
<script src="../includes/jquery-treeview/jquery.treeview.js" type="text/javascript"></script>

<script type="text/javascript"><!--
$(document).ready(function(){
	$("#tree").treeview({
		collapsed: true,
		animated: "medium",
		control:"#sidetreecontrol",
		persist: "cookie"
		
	});
	
});
-->
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width="100%" valign="top" style="background: none repeat scroll 0% 0%;" id="_arvore">
			<div id="main">

			<div id="sidetree">
			<div class="treeheader">&nbsp;</div>
			<div id="sidetreecontrol"><a href="?#">Fechar Todos</a> | <a href="?#">Abrir Todos</a> <br /><br /> <img src="../includes/dtree/img/base.gif" align="top" />Diagnóstico Par </span> </div>
			
			<ul id="tree" class="filetree treeview-famfamfam">
				<li><span class="folder"><strong>Home</strong></span>
					<ul>
						<li><a href="?/enewsletters/index.cfm">Airdrie eNewsletters </a></li>
						<li><a href="?/index.cfm">Airdrie Directories</a></li>
						<li><a href="?/economic_development/video/index.cfm">Airdrie Life Video</a></li>
					</ul>
				</li>
				<li><span class="folder"><strong>News</strong></span>
				<ul>
					<li><a href="?/enewsletters/index.cfm">Airdrie eNewsletters</a>
					<ul>
						<li><a href="?http://www.industrymailout.com/Industry/View.aspx?id=50169&amp;p=679b">Airdrie Today eNewsletter</a></li>
						<li><a href="?http://www.industrymailout.com/Industry/View.aspx?id=47265&amp;q=0&amp;qz=4c4af0">Airdrie @Work eNewsletter</a></li>
						<li><a href="?http://www.industrymailout.com/Industry/Archives.aspx?m=2682&amp;qz=73249dbb">Airdrie eNewsletter Archive</a></li>
			
					</ul>
					</li>
					<li><a href="?/calendars/index.cfm">Community Calendar</a></li>
					<li><a href="?/community_news/index.cfm">Community News</a></li>
					<li><a href="?/news_release/index.cfm">News Releases</a> (2007)
					<ul>
						<li><a href="?/news_release/2006/index.cfm" title="2006 News Releases">2006 News Releases</a></li>
			
						<li><a href="?/news_release/2005/index.cfm" title="2005 News Releases">2005 News Releases</a></li>
						<li><a href="?/news_release/2004/index.cfm" title="2004 News Releases">2004 News Releases</a></li>
					</ul>
					</li>
					<li><a href="?/building_development/planning/notice_of_development/notice_of_development.cfm">Notice of Development </a></li>
					<li><a href="?/photogallery/index.cfm">Photo Gallery</a></li>
					<li><a href="?/public_meetings/index.cfm">Public Meetings</a>
			
					<ul>
						<li><a href="?/public_meetings/appeals/index.cfm">Appeals</a></li>
						<li><a href="?/public_meetings/open_houses/index.cfm">Open Houses</a></li>
						<li><a href="?/public_meetings/public_hearings/index.cfm">Public Hearings</a></li>
					</ul>
					</li>
					<li><a href="?/publications/index.cfm">Publications</a>
			
					<ul>
						<li><a href="?/publications/pdf/AirdrieLIFE_fall2006.pdf">Airdrie Life Magazine</a> (16MB, .PDF)</li>
						<li><a href="?/publications/pdf/report_for_2005.pdf">Annual Economic Report</a> (5 MB, .PDF)</li>
						<li><a href="?/publications/pdf/Airdrie%20community%20report%20for%202006_sm.pdf">Annual Community Report</a></li>
					</ul>
			
					</li>
				</ul>
				</li>
				
				<li><strong><a href="?https://vch.airdrie.ca/index.cfm">Online Services</a></strong></li>
			
			</ul>
			</div>
			
			</div>
		</td>
	</tr>
</table>