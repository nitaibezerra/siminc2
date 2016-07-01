<?php


if(!$_SESSION['conid']){
	?>
	<script>
		alert("É necessário preencher a tela 'Cadastrar Termo de Compromisso.'\n ou sua sessão expirou. Por favor, entre novamente no PSE!");
		location.href='pse.php?modulo=principal/cadastroTermoCompromisso&acao=A';
	</script>
	<?
	exit;
}

if($_REQUEST['termo']){
	exibeTermo();
	exit;
}


function inserirDados(){
	global $db;
	
	
	$sql = "INSERT INTO pse.metapactuada
				(
	            	mpaacao06comp1,
					mpaacao01comp3,
					mpaacao02comp3,
					mpaacao03comp3,
					mpaacao04comp3,
	            	conid 
		        ) VALUES (
		        	'".$_POST['mpaacao06comp1']."',
		        	'".$_POST['mpaacao01comp3']."',
		        	'".$_POST['mpaacao02comp3']."',
		        	'".$_POST['mpaacao03comp3']."',
		        	'".$_POST['mpaacao04comp3']."',
		        	'".$_SESSION['conid']."'
		        ) ";

	$db->executar($sql);					
	$db->commit();
}

function alterarDados(){
	global $db;
	
	
	$sql = "UPDATE 
				pse.metapactuada
			SET 
				mpaacao06comp1 = '".$_POST['mpaacao06comp1']."',
				mpaacao01comp3 = '".$_POST['mpaacao01comp3']."',
				mpaacao02comp3 = '".$_POST['mpaacao02comp3']."',
				mpaacao03comp3 = '".$_POST['mpaacao03comp3']."',
				mpaacao04comp3 = '".$_POST['mpaacao04comp3']."'
			WHERE 
				mpaid = ".$_POST['mpaid'];

	$db->executar($sql);					
	$db->commit();
}






if($_POST){
	
	if(!$_POST['mpaid']){
		inserirDados();
	}
	else{
		alterarDados();
	}

	unset($_POST);
	print "<script>
			alert('Operação realizada com sucesso!');
			location.href='pse.php?modulo=principal/cadastroTermoPactuacao&acao=A';
		   </script>";

	exit();
	
}




$sql = "SELECT mundescricao as municipio, estuf as uf
		FROM territorios.municipio
		WHERE muncod = '".$_SESSION['pse']['muncod']."'";
$mun = $db->pegaLinha($sql);

//pega entid da SECRETARIA MUNICIPAL DE EDUCAÇÃO de um determinado municipio
/*
$sql = "select u.entid, e.entnome 
		from seguranca.usuario u
		left join entidade.entidade e on e.entid = u.entid
		where u.usucpf='".$_SESSION['usucpf']."'";
*/
$sql = "select et.entid, et.entnome from entidade.entidade et
		inner join entidade.endereco ed on ed.entid=et.entid
		inner join entidade.funcaoentidade fe on  fe.entid = et.entid AND fe.funid = 7
		where fe.fuestatus='A' and muncod = '".$_SESSION['pse']['muncod']."'
		order by 1 limit 1";
$secretaria = $db->pegaLinha($sql);

$entid = $secretaria['entid'];
$conano = $_SESSION["exercicio"];



// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';


$titulo = "PSE - Termo de Compromisso";
$dsctitulo = '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.<br>
<table width="100%" align="center"  cellpadding="2" cellspacing="1">
<tr>
	<td width="10%" style="text-align: right;" class="SubTituloDireita">UF:</td>
	<td width="90%" class="SubTituloEsquerda">'.$mun['uf'].'</td>
</tr>
<tr>
	<td style="text-align: right;" class="SubTituloDireita">MUNICÍPIO:</td>
	<td class="SubTituloEsquerda">'.$mun['municipio'].'</td>
</tr>
<tr>
	<td style="text-align: right;" class="SubTituloDireita">SECRETARIA:</td>
	<td class="SubTituloEsquerda">'.$secretaria['entnome'].'</td>
</tr>
</table>';

echo "<br>";

$menu = array(0 => array("id" => 1, "descricao" => "Termo de Compromisso", 			"link" => "/pse/pse.php?modulo=principal/cadastroTermoPactuacao&acao=A"),
					  1 => array("id" => 2, "descricao" => "Semana Saúde na Escola", 	"link" => "/pse/pse.php?modulo=principal/cadastroTermoSemana&acao=A")
				  	  );
		
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

monta_titulo( $titulo, $dsctitulo );
echo "<br>";

$db->cria_aba( $abacod_tela, $url, '');


if($_SESSION['conid']){
	$sql = "SELECT  m.mpaid,
					m.mpaacao06comp1,
					m.mpaacao01comp3,
					m.mpaacao02comp3,
					m.mpaacao03comp3,
					m.mpaacao04comp3,
					m.mpaacao05comp3
		     FROM 
		     		pse.metapactuada m
		     WHERE 
		     		m.conid = {$_SESSION['conid']}";
	$dados = $db->carregar($sql);
	if($dados) extract($dados[0]);
	
	$sql = "SELECT sum(pamquanteduseratend) as total FROM pse.planoacaoc1 
			WHERE conid = {$_SESSION['conid']}";
	$concoberturacomp1 = $db->pegaUm($sql);
	
	$sql = "SELECT sum(pacquanteduseratend) as total FROM pse.planoacaoc2 
			WHERE conid = {$_SESSION['conid']}";
	$concoberturacomp2 = $db->pegaUm($sql);
	
}

?>

<form id="formulario" name="formulario" action="" method="post">

<input type="hidden" name="mpaid" value="<?=$mpaid?>">

<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
<tr>
	<td class="SubTituloEsquerda" colspan='3'>
		<table>
			<tr>
				<td class="SubTituloEsquerda" valign="top" nowrap>
					 <br>
				     <b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Metas pactuadas por Componente / ação de implementação PSE</b>
				     <br><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; NO QUADRO ABAIXO ESTÃO IDENTIFICADAS AS METAS PACTUADAS PELO MUNICÍPIO. ATENTE-SE PORÉM QUE PARA A AÇÃO DO COMPONENTE I – AVALIAÇÃO AUDITIVA E PARA AS AÇÕES <br>
				      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; DO COMPONENTE III O MUNICÍPIO DEVERÁ INFORMAR A META A PARTIR DE SUA CAPACIDADE DE REALIZAÇÃO E COBERTURA: NÃO HÁ UM PARÂMETRO MÍNIMO PRÉ ESTABELECIDO.
				      <br><br>
				</td>
				<td>&nbsp;</td>
				<td valign="middle"  width="30%">
				
					<?php
				     
				     	$sql = "SELECT concoberturacomp1 as total1 FROM pse.contratualizacao 
								WHERE conid = {$_SESSION['conid']}";
						$total1 = $db->pegaUm($sql);
						if(!$total1) $total1 = 0;
					
						$sql = "SELECT sum(pamquanteduseratend) as total2 FROM pse.planoacaoc1 
								WHERE conid = {$_SESSION['conid']}";
						$total2 = $db->pegaUm($sql);
						if(!$total2) $total2 = 0;
				     
				     	$sql = "SELECT concoberturacomp2 as total1 FROM pse.contratualizacao 
								WHERE conid = {$_SESSION['conid']}";
						$total3 = $db->pegaUm($sql);
						if(!$total3) $total3 = 0;
						
						$sql = "SELECT sum(pacquanteduseratend) as total2 FROM pse.planoacaoc2 
								WHERE conid = {$_SESSION['conid']}";
						$total4 = $db->pegaUm($sql);
						if(!$total4) $total4 = 0;
						
						if( ($total1 == $total2 || $total1 < $total2) && ( $total3 == $total4 || $total3 < $total4) ){
				     		if($mpaacao06comp1 != '' && $mpaacao04comp3 != '' && $concoberturacomp1 != '' && $concoberturacomp2 != '' && $_SESSION['permissaoedicao']){?>
							     
							     	<font color=red><b>Você concluiu o preenchimento do Termo de Compromisso. Imprima- o  clicando no ícone da  impressora. Após a assinatura dos secretários de Saúde e de educação, deve-se scanear o documento, transformá-lo em arquivo único e anexá-lo no sistema.</b></font>
							     	<center>
							     	<img border="0" title="Imprimir Termo de Compromisso." src="../imagens/print.png" style="cursor: hand" onclick="window.open('pse.php?modulo=principal/cadastroTermoPactuacao&acao=A&termo=ok', 'termo', 'height=600, width=780, scrollbars=1');">
							     	</center>
							<?}
						}
					?>
				     <br><br>	
				
				</td>
			</tr>
	    </table>
	     
	</td>
</tr>
<tr>
	<td style="background: rgb(168, 168, 168);" colspan='3'><center>
		 <span style="font-size: 12"><b>COMPONENTE I - AVALIAÇÃO CLÍNICA E PSICOSOCIAL</b></span>
		 </center></td>
</tr>
<tr>
	<td width="45%" class="SubTituloCentro" >LINHA DE AÇÃO</td>
	<td width="45%" class="SubTituloCentro">AÇÃO</td>
	<td width="10%" class="SubTituloCentro" >META PACTUADA</td>
</tr>
<?php 
	$sql = "select laaid, laalinhaacao, laaacao, laacomponente, laastatus
			from pse.complinhaacao 
			where laacomponente = '1'
			and laastatus = 'A' ";
	$dados = $db->carregar($sql);
	foreach($dados as $d){
		?>
		<tr>
			<td width="45%" class="SubTituloEsquerda" ><?=$d['laalinhaacao']?></td>
			<td width="45%" class="SubTituloEsquerda"><?=$d['laaacao']?></td>
			<td width="10%" class="SubTituloCentro" nowrap="nowrap" >
				<?
					if($d['laaid'] == '6'){
						echo campo_texto( 'mpaacao06comp1', 'S', 'S', '', 18, 6, '######', '', 'right', '', 0, '', '', '', '');
					}else{
						echo $concoberturacomp1;
					}
				?>
			</td>
		</tr>
	<?}?>



<tr>
	<td style="background: rgb(168, 168, 168);" colspan='3'><center>
		 <span style="font-size: 12"><b>COMPONENTE II - PROMOÇÃO E PREVENÇÃO À SAÚDE</b></span>
		 </center></td>
</tr>
<tr>
	<td width="45%" class="SubTituloCentro" >LINHA DE AÇÃO</td>
	<td width="45%" class="SubTituloCentro">AÇÃO</td>
	<td width="10%" class="SubTituloCentro" >META PACTUADA</td>
</tr>
<?php 
	$sql = "select laaid, laalinhaacao, laaacao, laacomponente, laastatus
			from pse.complinhaacao 
			where laacomponente = '2'
			and laastatus = 'A' ";
	$dados = $db->carregar($sql);
	$i = 1;
	foreach($dados as $d){
		?>
		<tr>
			<?if($i==3){?>
				<td width="45%" class="SubTituloEsquerda" rowspan=2><?=$d['laalinhaacao']?></td>
			<?}elseif($i!=3 && $i!=4){?>
				<td width="45%" class="SubTituloEsquerda" ><?=$d['laalinhaacao']?></td>
			<?}?>
			<td width="45%" class="SubTituloEsquerda"><?=$d['laaacao']?></td>
			<td width="10%" class="SubTituloCentro" nowrap="nowrap" >
			<?php if( $d['laaid'] == '13' ): ?>
			<?php echo campo_texto( 'mpaacao04comp3', 'S', 'S', '', 18, 6, '######', '', 'right', '', 0, '', '', '', ''); ?>
			<?php else: ?>
			<? echo $concoberturacomp2; ?>
			</td>
			<?php endif; ?>
		</tr>
		<?
		$i++;
	}
	?>


<tr>
	<td style="background: rgb(168, 168, 168);" colspan='3'><center>
		 <span style="font-size: 12"><b>COMPONENTE III - EDUCAÇÃO PERMANENTE</b></span>
		 </center></td>
</tr>
<tr>
	<td width="45%" class="SubTituloCentro" >LINHA DE AÇÃO</td>
	<td width="45%" class="SubTituloCentro">AÇÃO</td>
	<td width="10%" class="SubTituloCentro" >META PACTUADA</td>
</tr>
<?php 
	$sql = "select laaid, laalinhaacao, laaacao, laacomponente, laastatus
			from pse.complinhaacao 
			where laacomponente = '3'
			and laastatus = 'A' ";
	$dados = $db->carregar($sql);
	$i = 1; 
	foreach($dados as $d){
		?>
		<tr>
			<?if($i==1){?>
				<td width="45%" class="SubTituloEsquerda" rowspan=3><?=$d['laalinhaacao']?></td>
			<?}elseif($i>3){?>
				<td width="45%" class="SubTituloEsquerda" ><?=$d['laalinhaacao']?></td>
			<?}?>
			<td width="45%" class="SubTituloEsquerda"><?=$d['laaacao']?></td>
			<td width="10%" class="SubTituloCentro" nowrap="nowrap" >
				<?
					echo campo_texto( 'mpaacao0'.$i.'comp3', 'S', 'S', '', 18, 6, '######', '', 'right', '', 0, '', '', '', '');
				?>
			</td>
		</tr>
		<?
		$i++;
	}
	?>


<tr>
	<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='3'>
		<input type="button" class="botao" name="btSalvar" id="btSalvar" value="Salvar" onclick="validaForm();">
	</td>
</tr>
</table>


</form>

<?php /*
			$sql = "SELECT
						'<a href=\"#\" onclick=\"alterarComp(\'' || pc.pamid || '\',\'1\');\" title=\"Alterar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\" onclick=\"excluirComp(\'' || pc.pamid || '\',\'1\');\" title=\"Excluir\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>' as acao, 
						ent.entnome,
		            	pamquantedutotal,
		            	pamquanteduseratend, 
		            	sc.cnenomefantasia,
						pampercentuseratend
					FROM pse.planoacaoc1 pc
					INNER JOIN entidade.entidade ent on ent.entid=pc.entid
					INNER JOIN pse.scnes sc on sc.cneid=pc.cneid
					WHERE pc.conid = {$_SESSION['conid']}";
			
			$cabecalho 	= array( "Ação", "Escola","Número de Educandos Total", "Número de Educandos a Serem Atendidos pelo PSE","Equipe CNES", "% Percentual de Atendimento");
			$alinha = array("center","left","center","center","left","center");
			$tamanho = array("5%","25%","10%","20%","20%","10%");
			$db->monta_lista( $sql, $cabecalho, 500, 10, 'N', 'center', '', '',$tamanho,$alinha,'form1');
		*/				
?>

<script type="text/javascript">

var d = document.formulario;

function validaForm(){

	var obj = document.getElementsByTagName("input");
	var total = document.getElementsByTagName("input").length;
	var flagvalida = 0;

	for(i=0; i<total; i++){
		if(obj[i].type == 'text' && obj[i].value == ''){
			flagvalida = 1;
			break;
		}
	}

	if(flagvalida == 1){
		alert ('Favor Preencher todos os campos obrigatórios.');
		obj[i].focus();
		return false;
	}
	
	d.submit();

}

<?php if($_SESSION['permissaoedicao'] == false){?>
	var obj = document.getElementsByTagName("input");
	var total = document.getElementsByTagName("input").length;
	
	for(i=0; i<total; i++){
		obj[i].disabled = true;
	}
<?php }?>

</script>


<?php 

function exibeTermo(){
	global $db;
	
	$sql = "SELECT mundescricao ||'/'|| estuf as municipio FROM territorios.municipio
			WHERE muncod = '".$_SESSION['pse']['muncod']."'";
	$municipio = $db->pegaUm($sql);
	
	$sql = "SELECT
            	entid,
            	conano, 
            	conseendereco, 
            	consetelefone, 
            	conseemailsecretaria, 
	            consenomesecretario, 
	            consecpf, 
	            conseemailsecretario, 
	            conssendereco, 
	            consstelefone, 
	            conssemailsecretaria, 
	            conssnomesecretario, 
	            consscpf, 
	            conssemailsecretario, 
	            congtisenome, 
	            congtisecpf, 
	            congtisecargofuncao, 
	            congtisetelefone, 
	            congtiseemail, 
	            congtissnome, 
	            congtisscpf, 
	            congtisscargofuncao, 
	            congtisstelefone, 
	            congtissemail, 
	            consecnpj, 
	            conseidentidade, 
	            conseorgaoexp, 
	            conseestadocivil, 
	            consscnpj, 
	            conssidentidade, 
	            conssorgaoexp, 
	            conssestadocivil,
	            conesfatuarapse,
				concoberturacomp1,
				conpercentualcomp1,
				concoberturacomp2,
				conpercentualcomp2
			FROM 
				pse.contratualizacao	
			WHERE 
				conid = {$_SESSION['conid']}";
	$dados = $db->pegaLinha($sql);
	if($dados) extract($dados);
	
	
	?>
	<html xmlns:o="urn:schemas-microsoft-com:office:office"
xmlns:w="urn:schemas-microsoft-com:office:word"
xmlns="http://www.w3.org/TR/REC-html40">
 
<head>
<meta http-equiv=Content-Type content="text/html; charset=us-ascii">
<meta name=ProgId content=Word.Document>
<meta name=Generator content="Microsoft Word 11">
<meta name=Originator content="Microsoft Word 11">
<link rel=File-List href="ANEXOITermodecompromissoWEB_arquivos/filelist.xml">
<title>TERMO DE COMPROMISSO MUNICIPAL</title>
<!--[if gte mso 9]><xml>
 <o:DocumentProperties>
  <o:Author>raquel.pedroso</o:Author>
  <o:Template>Normal</o:Template>
  <o:LastAuthor>DTI</o:LastAuthor>
  <o:Revision>2</o:Revision>
  <o:TotalTime>32</o:TotalTime>
  <o:LastPrinted>2011-08-09T13:21:00Z</o:LastPrinted>
  <o:Created>2011-08-09T22:14:00Z</o:Created>
  <o:LastSaved>2011-08-09T22:14:00Z</o:LastSaved>
  <o:Pages>1</o:Pages>
  <o:Words>1994</o:Words>
  <o:Characters>10772</o:Characters>
  <o:Company>MEC</o:Company>
  <o:Lines>89</o:Lines>
  <o:Paragraphs>25</o:Paragraphs>
  <o:CharactersWithSpaces>12741</o:CharactersWithSpaces>
  <o:Version>11.5606</o:Version>
 </o:DocumentProperties>
</xml><![endif]--><!--[if gte mso 9]><xml>
 <w:WordDocument>
  <w:HyphenationZone>21</w:HyphenationZone>
  <w:DoNotHyphenateCaps/>
  <w:PunctuationKerning/>
  <w:ValidateAgainstSchemas>false</w:ValidateAgainstSchemas>
  <w:SaveIfXMLInvalid>false</w:SaveIfXMLInvalid>
  <w:IgnoreMixedContent>false</w:IgnoreMixedContent>
  <w:AlwaysShowPlaceholderText>false</w:AlwaysShowPlaceholderText>
  <w:DoNotUnderlineInvalidXML/>
  <w:Compatibility>
   <w:BreakWrappedTables/>
   <w:SnapToGridInCell/>
   <w:WrapTextWithPunct/>
   <w:UseAsianBreakRules/>
   <w:DontGrowAutofit/>
  </w:Compatibility>
  <w:BrowserLevel>MicrosoftInternetExplorer4</w:BrowserLevel>
 </w:WordDocument>
</xml><![endif]--><!--[if gte mso 9]><xml>
 <w:LatentStyles DefLockedState="false" LatentStyleCount="156">
  <w:LsdException Locked="true" Name="Normal"/>
  <w:LsdException Locked="true" Name="heading 1"/>
  <w:LsdException Locked="true" Name="heading 2"/>
  <w:LsdException Locked="true" Name="heading 3"/>
  <w:LsdException Locked="true" Name="heading 4"/>
  <w:LsdException Locked="true" Name="heading 5"/>
  <w:LsdException Locked="true" Name="heading 6"/>
  <w:LsdException Locked="true" Name="heading 7"/>
  <w:LsdException Locked="true" Name="heading 8"/>
  <w:LsdException Locked="true" Name="heading 9"/>
  <w:LsdException Locked="true" Name="toc 1"/>
  <w:LsdException Locked="true" Name="toc 2"/>
  <w:LsdException Locked="true" Name="toc 3"/>
  <w:LsdException Locked="true" Name="toc 4"/>
  <w:LsdException Locked="true" Name="toc 5"/>
  <w:LsdException Locked="true" Name="toc 6"/>
  <w:LsdException Locked="true" Name="toc 7"/>
  <w:LsdException Locked="true" Name="toc 8"/>
  <w:LsdException Locked="true" Name="toc 9"/>
  <w:LsdException Locked="true" Name="caption"/>
  <w:LsdException Locked="true" Name="Title"/>
  <w:LsdException Locked="true" Name="Subtitle"/>
  <w:LsdException Locked="true" Name="Strong"/>
  <w:LsdException Locked="true" Name="Emphasis"/>
  <w:LsdException Locked="true" Name="Table Grid"/>
 </w:LatentStyles>
</xml><![endif]-->
<style> 
<!--
 /* Font Definitions */
 @font-face
	{font-family:Calibri;
	panose-1:2 15 5 2 2 2 4 3 2 4;
	mso-font-charset:0;
	mso-generic-font-family:swiss;
	mso-font-pitch:variable;
	mso-font-signature:-1610611985 1073750139 0 0 159 0;}
 /* Style Definitions */
 p.MsoNormal, li.MsoNormal, div.MsoNormal
	{mso-style-parent:"";
	margin:0cm;
	margin-bottom:.0001pt;
	mso-pagination:widow-orphan;
	font-size:12.0pt;
	font-family:"Times New Roman";
	mso-fareast-font-family:Calibri;}
p.MsoBodyText, li.MsoBodyText, div.MsoBodyText
	{mso-style-noshow:yes;
	mso-style-link:"Body Text Char";
	margin-top:0cm;
	margin-right:0cm;
	margin-bottom:6.0pt;
	margin-left:0cm;
	mso-pagination:widow-orphan;
	font-size:12.0pt;
	font-family:"Times New Roman";
	mso-fareast-font-family:Calibri;}
p.TitArial1, li.TitArial1, div.TitArial1
	{mso-style-name:"Tit Arial 1";
	mso-style-next:"Corpo de texto";
	margin:0cm;
	margin-bottom:.0001pt;
	mso-pagination:widow-orphan;
	font-size:12.0pt;
	font-family:"Times New Roman";
	mso-fareast-font-family:Calibri;}
span.BodyTextChar
	{mso-style-name:"Body Text Char";
	mso-style-noshow:yes;
	mso-style-locked:yes;
	mso-style-link:"Corpo de texto";
	mso-ansi-font-size:12.0pt;
	mso-bidi-font-size:12.0pt;
	font-family:"Times New Roman";
	mso-ascii-font-family:"Times New Roman";
	mso-hansi-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	mso-ansi-language:X-NONE;
	mso-fareast-language:PT-BR;}
@page Section1
	{size:595.3pt 841.9pt;
	margin:70.85pt 3.0cm 70.85pt 3.0cm;
	mso-header-margin:35.4pt;
	mso-footer-margin:35.4pt;
	mso-paper-source:0;}
div.Section1
	{page:Section1;}
-->
</style>
<!--[if gte mso 10]>
<style>
 /* Style Definitions */
 table.MsoNormalTable
	{mso-style-name:"Tabela normal";
	mso-tstyle-rowband-size:0;
	mso-tstyle-colband-size:0;
	mso-style-noshow:yes;
	mso-style-parent:"";
	mso-padding-alt:0cm 5.4pt 0cm 5.4pt;
	mso-para-margin:0cm;
	mso-para-margin-bottom:.0001pt;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Calibri;
	mso-ansi-language:#0400;
	mso-fareast-language:#0400;
	mso-bidi-language:#0400;}
</style>
<![endif]-->
</head>
 
<body lang=PT-BR style='tab-interval:35.4pt'>
 
<div class=Section1>
 
<p class=MsoNormal style='margin-left:155.95pt;text-align:justify'><b>TERMO DE
COMPROMISSO MUNICIPAL<o:p></o:p></b></p>
 
<p class=MsoNormal style='margin-left:155.95pt;text-align:justify'><o:p>&nbsp;</o:p></p>
 
<p class=MsoNormal style='margin-left:155.95pt;text-align:justify'>Termo de
Compromisso Municipal que firmam as Secretarias Municipais de Sa&uacute;de e de
Educa&ccedil;&atilde;o de <b><?=$municipio?></b>, representadas pelos (as) seus
Secret&aacute;rios (as) Municipais de Sa&uacute;de e Educa&ccedil;&atilde;o,
para pactuar e formalizar as responsabilidades e metas inerentes &agrave;
execu&ccedil;&atilde;o do Programa Sa&uacute;de na Escola, nos
territ&oacute;rios de responsabilidades, objetivando o desenvolvimento <span
style='color:black'>das a&ccedil;&otilde;es de preven&ccedil;&atilde;o,
promo&ccedil;&atilde;o e aten&ccedil;&atilde;o &agrave; sa&uacute;de dos
escolares, para recebimento de recursos financeiros.<o:p></o:p></span></p>
 
<p class=MsoNormal style='text-align:justify;text-indent:70.8pt'><span
style='color:black'><o:p>&nbsp;</o:p></span></p>
 
<p class=MsoNormal style='text-align:justify;mso-layout-grid-align:none;
text-autospace:none'><span style='color:black'><span style='mso-tab-count:2'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
 
<p class=MsoNormal style='text-align:justify;mso-layout-grid-align:none;
text-autospace:none'><span style='color:black'><span style='mso-tab-count:2'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span>O
Governo Municipal de <b><?=$municipio?></b>, por interm&eacute;dio das suas Secretarias </span>Municipais
de Sa&uacute;de, inscrita no CNPJ sob n&ordm; <b><?=formatar_cpf_cnpj($consscnpj)?></b> e de
Educa&ccedil;&atilde;o, inscrita no CNPJ sob n&ordm; <b><?=formatar_cpf_cnpj($consecnpj)?></b>,<span
style='mso-spacerun:yes'>&nbsp; </span><span style='color:black'>neste ato
representado pelos seus Secret&aacute;rios de Sa&uacute;de, <b><?=strtoupper($conssnomesecretario)?></b>, <b><?=$conssestadocivil?></b>, portador da carteira de identidade n&ordm; <b><?=$conssidentidade?></b>, expedida por <b><?=strtoupper($conssorgaoexp)?></b>, e inscrito no CPF sob o n&ordm; <b><?=formatar_cpf_cnpj($consscpf)?></b> 
e Educa&ccedil;&atilde;o, <b><?=strtoupper($consenomesecretario)?></b>, <b><?=$conseestadocivil?></b>, portador da carteira de identidade n&ordm; <b><?=$conseidentidade?></b>, expedida por <b><?=strtoupper($conseorgaoexp)?></b>, e inscrito no CPF sob o n&ordm; <b><?=formatar_cpf_cnpj($consecpf)?></b>, considerando o que
disp&otilde;e a Constitui&ccedil;&atilde;o Federal, em especial os seus artigos
196 e 205, as Leis n&ordm; 8.080/90 e n&ordm; 8.142/90 e considerando o que
disp&otilde;e o Decreto Presidencial<span style='mso-spacerun:yes'>&nbsp;
</span></span>n&ordm; 6.286, de 5 de dezembro de 2007, que institui o Programa
Sa&uacute;de na Escola &#8211; PSE, celebra o presente Termo de Compromisso
Municipal, formalizando as metas das a&ccedil;&otilde;es <span
style='color:black'>de preven&ccedil;&atilde;o, promo&ccedil;&atilde;o e
aten&ccedil;&atilde;o &agrave; sa&uacute;de dos escolares, nos seus
territ&oacute;rios de responsabilidades.</span></p>
 
<p class=MsoNormal style='text-align:justify;text-indent:70.8pt'><span
style='color:black'><o:p>&nbsp;</o:p></span></p>
 
<p class=MsoNormal style='text-align:justify;text-indent:70.8pt'><span
style='color:black'><o:p>&nbsp;</o:p></span></p>
 
<p class=MsoNormal style='margin-left:35.4pt;text-align:justify;text-indent:
35.4pt'><b><span style='color:black'>CL&Aacute;USULA PRIMEIRA - DO OBJETO<o:p></o:p></span></b></p>
 
<p class=MsoNormal style='text-align:justify'><span style='color:black'><span
style='mso-tab-count:2'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span>O
presente Termo de Compromisso </span>Gest&atilde;o Municipal do Programa
Sa&uacute;de na Escola (PSE)<span style='color:black'> tem por objetivo a
conjuga&ccedil;&atilde;o de esfor&ccedil;os visando &agrave;
preven&ccedil;&atilde;o, promo&ccedil;&atilde;o e aten&ccedil;&atilde;o
&agrave; sa&uacute;de dos escolares do Programa Sa&uacute;de na Escola,
articulada de forma intersetorial entre as redes de sa&uacute;de e de
educa&ccedil;&atilde;o.<o:p></o:p></span></p>
 
<p class=MsoNormal style='text-align:justify'><span style='color:black'><o:p>&nbsp;</o:p></span></p>
 
<p class=MsoNormal style='margin-left:35.4pt;text-align:justify;text-indent:
35.4pt'><b><span style='color:black'>CL&Aacute;USULA SEGUNDA &#8211; DO REPASSE
DO RECURSO FINANCEIRO<o:p></o:p></span></b></p>
 
<p class=MsoNormal style='text-align:justify;text-indent:71.4pt'>A
transfer&ecirc;ncia do recurso financeiro se efetivar&aacute; por meio de
repasse fundo a fundo na modalidade PAB vari&aacute;vel, compondo o Bloco de
Financiamento da Aten&ccedil;&atilde;o B&aacute;sica do Pacto pela
Sa&uacute;de.</p>
 
<p class=MsoNormal style='text-align:justify;text-indent:71.4pt'><o:p>&nbsp;</o:p></p>
 
<p class=MsoNormal style='text-align:justify;text-indent:71.4pt'>&sect; 1&ordm;
O valor total recurso financeiro corresponde a uma parcela extra do incentivo
mensal das Equipes de Sa&uacute;de de Fam&iacute;lia que atuam no Programa
Sa&uacute;de na Escola (PSE).</p>
 
<p class=MsoNormal style='text-align:justify;text-indent:71.4pt'>&sect; 2&ordm;
70% do valor total do recurso financeiro do Programa Sa&uacute;de na Escola
(PSE)<span style='color:black'> </span>ser&aacute; pago a partir da assinatura
do <span style='color:black'>Termo de Compromisso Municipal</span> do Programa
Sa&uacute;de na Escola (PSE). Os 30% restantes do valor total do recurso
financeiro ser&aacute; pago ap&oacute;s o cumprimento de 70% das metas
municipais pactuadas.</p>
 
<p class=MsoNormal style='text-align:justify;text-indent:71.4pt'>&sect; 3&ordm;
Os Munic&iacute;pios que atingirem 70% das metas pactuadas, no prazo de 12
meses da data de assinatura do Termo de Compromisso Municipal, estar&atilde;o
aptos &agrave; formaliza&ccedil;&atilde;o de novo Termo para o pr&oacute;ximo
exerc&iacute;cio.</p>
 
<p class=MsoNormal style='text-align:justify;text-indent:71.4pt'><o:p>&nbsp;</o:p></p>
 
<p class=MsoNormal style='text-align:justify;text-indent:71.4pt'><span
style='mso-spacerun:yes'>&nbsp;</span>&sect; 4&ordm; Os Munic&iacute;pios que
n&atilde;o atingirem os 70% das metas pactuadas, no prazo de 12 meses da data
de assinatura do Termo de Compromisso Municipal, perder&atilde;o os 30% do
total do incentivo financeiro.</p>
 
<p class=MsoNormal style='text-align:justify;text-indent:71.4pt'>I. Os
Munic&iacute;pios estar&atilde;o aptos &agrave; formaliza&ccedil;&atilde;o de
novo Termo de Compromisso Municipal quando atingirem 70% das metas pactuadas.</p>
 
<p class=MsoNormal style='text-align:justify;text-indent:71.4pt'><o:p>&nbsp;</o:p></p>
 
<p class=MsoNormal style='text-align:justify;text-indent:71.4pt'>&sect; 5&ordm;
cabe ao Grupo de Trabalho Intersetorial Municipal (GTI) a gest&atilde;o do
recurso financeiro do PSE.</p>
 
<p class=MsoNormal style='text-align:justify;text-indent:71.4pt'><o:p>&nbsp;</o:p></p>
 
<p class=MsoNormal style='text-align:justify;text-indent:71.4pt'>&sect; 6&ordm;
Para o alcance das metas pactuadas, a vincula&ccedil;&atilde;o territorial das
Equipes Sa&uacute;de da Fam&iacute;lia com as Escolas no planejamento das
a&ccedil;&otilde;es de preven&ccedil;&atilde;o, promo&ccedil;&atilde;o e
aten&ccedil;&atilde;o &agrave; sa&uacute;de dos escolares, nos
territ&oacute;rios de responsabilidades, poder&aacute; ser revista a qualquer
tempo, sem preju&iacute;zo das metas pactuadas. </p>
 
<p class=MsoNormal style='text-align:justify;text-indent:71.4pt'><o:p>&nbsp;</o:p></p>
 
<p class=MsoNormal style='text-align:justify'><b><o:p>&nbsp;</o:p></b></p>
 
<p class=MsoNormal style='margin-left:35.4pt;text-align:justify;text-indent:
35.4pt'><b>CL&Aacute;USULA TERCEIRA &#8211; DO PAR&Acirc;METRO ESSENCIAL<o:p></o:p></b></p>
 
<p class=MsoNormal style='text-align:justify'><b><span style='mso-tab-count:
2'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><o:p></o:p></b></p>
 
<p class=MsoNormal style='text-align:justify;text-indent:70.8pt'>Ficam
definidos, por Componente, os Par&acirc;metros Essenciais de cobertura das
a&ccedil;&otilde;es de preven&ccedil;&atilde;o, promo&ccedil;&atilde;o e
aten&ccedil;&atilde;o &agrave; sa&uacute;de dos escolares do Programa
Sa&uacute;de na Escola (PSE), sendo eles: 500 escolares/ano por Equipe de
Sa&uacute;de da Fam&iacute;lia para as a&ccedil;&otilde;es do Componente I
&#8211; Avalia&ccedil;&atilde;o Cl&iacute;nica e psicossocial e 1000
escolares/ano ou 100% dos escolares em caso inferior a 1000 escolares no
munic&iacute;pio, por Equipe de Sa&uacute;de da Fam&iacute;lia, para as
a&ccedil;&otilde;es do Componente II &#8211; Promo&ccedil;&atilde;o e
preven&ccedil;&atilde;o &agrave; sa&uacute;de.</p>
 
<p class=MsoNormal style='text-align:justify;text-indent:70.8pt'>&sect; 1&ordm;
o par&acirc;metro m&iacute;nimo ser&aacute; calculado a partir da
informa&ccedil;&atilde;o<span style='color:black'> do munic&iacute;pio do
n&uacute;mero de Equipes da Sa&uacute;de da Fam&iacute;lia que ir&atilde;o
atuar na implementa&ccedil;&atilde;o das a&ccedil;&otilde;es do Programa
Sa&uacute;de na Escola (PSE).<o:p></o:p></span></p>
 
<p class=MsoNormal style='text-align:justify;text-indent:70.8pt'><span
style='color:black'>&sect; 2&ordm; as metas anuais pactuadas pelos
munic&iacute;pios dever&atilde;o ser iguais ou maiores ao par&acirc;metro m&iacute;nimo
calculado.<o:p></o:p></span></p>
 
<p class=MsoNormal style='text-align:justify'><b><span style='color:black'><o:p>&nbsp;</o:p></span></b></p>
 
<p class=MsoNormal style='text-align:justify'><b><span style='color:black'><o:p>&nbsp;</o:p></span></b></p>
 
<p class=MsoNormal style='text-align:justify'><b><span style='color:black'><span
style='mso-tab-count:2'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span>CL&Aacute;USULA
QUARTA &#8211; DA VIG&Ecirc;NCIA</span></b><span style='color:black'><span
style='mso-tab-count:1'> </span><o:p></o:p></span></p>
 
<p class=MsoNormal style='text-align:justify'><span style='color:black'><span
style='mso-tab-count:2'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
 
<p class=MsoNormal style='text-align:justify;text-indent:70.8pt'><span
style='color:black'>O prazo para realiza&ccedil;&atilde;o das metas das
a&ccedil;&otilde;es pactuadas no presente instrumento tem a vig&ecirc;ncia de
doze meses a contar da data de assinatura deste instrumento.</span></p>
 
<p class=MsoNormal style='text-align:justify'><b><span style='color:black'><o:p>&nbsp;</o:p></span></b></p>
 
<p class=MsoNormal style='text-align:justify'><b><span style='color:black'><o:p>&nbsp;</o:p></span></b></p>
 
<p class=MsoNormal style='text-align:justify'><b><span style='color:black'><span
style='mso-tab-count:2'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span>CL&Aacute;USULA
QUINTA - DAS ATRIBUI&Ccedil;&Otilde;ES E RESPONSABILIDADES DA ESFERA MUNICIPAL
E/OU DISTRITAL<o:p></o:p></span></b></p>
 
<p class=TitArial1 style='text-autospace:ideograph-other'><span
style='mso-tab-count:2'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span></p>
 
<p class=TitArial1 style='text-autospace:ideograph-other'><b><span
style='color:black'><span style='mso-tab-count:2'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span>A
Secretaria Municipal de Sa&uacute;de de <b><?=$municipio?></b> e a Secretaria Municipal de
Educa&ccedil;&atilde;o de <b><?=$municipio?></b> comprometem-se a:</span><o:p></o:p></b></p>
 
<p class=TitArial1 style='text-autospace:ideograph-other'><b><span
style='color:black'><o:p>&nbsp;</o:p></span></b></p>
 
<p class=MsoNormal style='text-align:justify;mso-hyphenate:none;tab-stops:21.3pt'>a)constituir
GTI &#8211; Grupo de Trabalho Intersetorial Municipal de
execu&ccedil;&atilde;o, gest&atilde;o do recurso, acompanhamento e
monitoramento das a&ccedil;&otilde;es do PSE no territ&oacute;rio, com
representantes da sa&uacute;de, educa&ccedil;&atilde;o e de outros
&oacute;rg&atilde;os/institui&ccedil;&otilde;es.</p>
 
<p class=MsoNormal style='text-align:justify;mso-hyphenate:none;tab-stops:21.3pt'><span
style='mso-spacerun:yes'>&nbsp;</span></p>
 
<p class=MsoNormal style='text-align:justify;mso-hyphenate:none;tab-stops:21.3pt'>b)incluir
os temas contemplados no <span style='color:black'>Programa Sa&uacute;de na
Escola (</span>PSE) nos projetos pol&iacute;tico-pedag&oacute;gicos das escolas
envolvidas no programa;</p>
 
<p class=MsoNormal style='text-align:justify;mso-hyphenate:none;tab-stops:21.3pt'><o:p>&nbsp;</o:p></p>
 
<p class=MsoNormal style='text-align:justify;mso-hyphenate:none;tab-stops:21.3pt'>c)definir
conjuntamente as escolas federais, estaduais e municipais<span
style='mso-spacerun:yes'>&nbsp; </span>a serem atendidas no &acirc;mbito do <span
style='color:black'>Programa Sa&uacute;de na Escola (</span>PSE),<span
style='color:navy'> </span>considerando os territ&oacute;rios de
abrang&ecirc;ncia das Unidades B&aacute;sicas de Sa&uacute;de, o n&uacute;mero
de Equipes de Sa&uacute;de da Fam&iacute;lia implantadas e o Par&acirc;metro
Essencial de cobertura das a&ccedil;&otilde;es do <span style='color:black'>Programa
Sa&uacute;de na Escola (</span>PSE);</p>
 
<p class=MsoNormal style='text-align:justify;mso-hyphenate:none;tab-stops:21.3pt'><o:p>&nbsp;</o:p></p>
 
<p class=MsoNormal style='text-align:justify;mso-hyphenate:none;tab-stops:21.3pt'>d)definir
as metas de cobertura anual das a&ccedil;&otilde;es previstas no <span
style='color:black'>Programa Sa&uacute;de na Escola (</span>PSE),<span
style='mso-spacerun:yes'>&nbsp; </span>considerando o Par&acirc;metro Essencial
de cada Componente (I e II);</p>
 
<p class=MsoNormal style='text-align:justify;mso-hyphenate:none;tab-stops:21.3pt'><o:p>&nbsp;</o:p></p>
 
<p class=MsoNormal style='text-align:justify;mso-hyphenate:none;tab-stops:21.3pt'>e)definir
as estrat&eacute;gias espec&iacute;ficas de coopera&ccedil;&atilde;o entre
estados e munic&iacute;pios para gest&atilde;o do cuidado dos escolares
identificados com necessidades de sa&uacute;de;</p>
 
<p class=MsoNormal style='text-align:justify;mso-hyphenate:none;tab-stops:21.3pt'><o:p>&nbsp;</o:p></p>
 
<p class=MsoNormal style='text-align:justify'>f)preencher sistematicamente o
sistema de monitoramento e avalia&ccedil;&atilde;o do PSE, com as
informa&ccedil;&otilde;es das a&ccedil;&otilde;es realizadas e seus respectivos
resultados;</p>
 
<p class=MsoNormal style='text-align:justify'><o:p>&nbsp;</o:p></p>
 
<p class=MsoNormal style='text-align:justify'>g)garantir a entrega dos
materiais do PSE, enviados pelo Minist&eacute;rio da Educa&ccedil;&atilde;o,
para as Equipes de Sa&uacute;de e para as Escolas. </p>
 
<p class=MsoNormal style='text-align:justify'><o:p>&nbsp;</o:p></p>
 
<p class=MsoNormal style='text-align:justify;mso-layout-grid-align:none;
text-autospace:none'>h)encaminhar o presente Termo de Compromisso Municipal de
Gest&atilde;o do Programa Sa&uacute;de na Escola (PSE) , aos Conselhos
Municipais de Sa&uacute;de e Educa&ccedil;&atilde;o, quando houver, e &agrave;
Comiss&atilde;o Intergestores Bipartite (CIB) de seu Estado para
homologa&ccedil;&atilde;o.</p>
 
<p class=MsoNormal style='text-align:justify;mso-hyphenate:none'><o:p>&nbsp;</o:p></p>
 
<p class=TitArial1 style='text-align:justify;text-autospace:ideograph-other'>i)
as atribui&ccedil;&otilde;es e responsabilidades de gest&atilde;o Municipal do
Programa Sa&uacute;de na Escola (PSE) contidas neste Termo ser&atilde;o
pactuadas mediante o preenchimento das informa&ccedil;&otilde;es, conforme
descritas nos quadros abaixo, exclusivamente no Sistema de Monitoramento e
Avalia&ccedil;&atilde;o do PSE: </p>
 
<p class=MsoNormal style='text-align:justify;text-indent:35.4pt'>- Cadastro das
informa&ccedil;&otilde;es das Secretarias Municipais da Educa&ccedil;&atilde;o
e Sa&uacute;de;</p>
 
<p class=MsoNormal style='text-align:justify;text-indent:35.4pt'>- Cadastro das
informa&ccedil;&otilde;es dos representantes nos GTIs das Secretarias
Municipais da Educa&ccedil;&atilde;o e Sa&uacute;de;</p>
 
<p class=MsoNormal style='text-align:justify;text-indent:35.4pt'>-
Identifica&ccedil;&atilde;o das Equipes de Sa&uacute;de da Fam&iacute;lia (ESF)
que atuar&atilde;o no PSE para c&aacute;lculo de meta m&iacute;nima de
cobertura;</p>
 
<p class=MsoNormal style='text-align:justify;text-indent:35.4pt'>-
Vincula&ccedil;&atilde;o das ESF com as escolas do territ&oacute;rio de
responsabilidade compartilhada;</p>
 
<p class=MsoNormal style='text-align:justify;text-indent:35.4pt'>-
Conclus&atilde;o da pactua&ccedil;&atilde;o das metas por a&ccedil;&atilde;o
dos Componentes: I - Avalia&ccedil;&atilde;o Cl&iacute;nica e Psicossocial; II
- Promo&ccedil;&atilde;o da Sa&uacute;de e Preven&ccedil;&atilde;o e III -
Educa&ccedil;&atilde;o Permanente e Capacita&ccedil;&atilde;o de Profissionais
da Educa&ccedil;&atilde;o e da Sa&uacute;de e de Jovens para o PSE.</p>
 
<p class=MsoNormal align=center style='text-align:center;mso-layout-grid-align:
none;text-autospace:none'><span style='color:black'><o:p>&nbsp;</o:p></span></p>
 
<p class=TitArial1 style='text-autospace:ideograph-other'><b><span
style='color:black'><o:p>&nbsp;</o:p></span></b></p>
 
<table class=MsoNormalTable border=1 cellspacing=0 cellpadding=0
 style='border-collapse:collapse;border:none;mso-border-alt:solid windowtext .5pt;
 mso-yfti-tbllook:160;mso-padding-alt:0cm 5.4pt 0cm 5.4pt;mso-border-insideh:
 .5pt solid windowtext;mso-border-insidev:.5pt solid windowtext'>
 
 <!-- 
 <tr style='mso-yfti-irow:0;mso-yfti-firstrow:yes'>
  <td width=581 colspan=6 valign=top style='width:435.5pt;border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>
  <p class=TitArial1 style='text-autospace:ideograph-other'>QUADRO I:
  VINCULA&Ccedil;&Atilde;O PSE TERRITORIAL ESF - ESCOLA</p>
  </td>
 </tr>
 <tr style='mso-yfti-irow:1'>
  <td width=100 valign=top style='width:75.3pt;border:solid windowtext 1.0pt;
  border-top:none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 5.4pt 0cm 5.4pt'>
  <p class=MsoNormal style='mso-layout-grid-align:none;text-autospace:none'><b>C&oacute;digo
  IBGE do<o:p></o:p></b></p>
  <p class=MsoNormal style='mso-layout-grid-align:none;text-autospace:none'><b>Munic&iacute;pio<o:p></o:p></b></p>
  <p class=TitArial1 style='text-autospace:ideograph-other'><span
  style='color:black'><o:p>&nbsp;</o:p></span></p>
  </td>
  <td width=96 valign=top style='width:72.0pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>
  <p class=TitArial1 style='text-autospace:ideograph-other'>Nome do
  Munic&iacute;pio<span style='color:black'><o:p></o:p></span></p>
  </td>
  <td width=96 valign=top style='width:72.05pt;border-top:none;border-left:
  none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>
  <p class=MsoNormal style='mso-layout-grid-align:none;text-autospace:none'><b>C&oacute;digo
  CNES da<o:p></o:p></b></p>
  <p class=MsoNormal style='mso-layout-grid-align:none;text-autospace:none'><b>ESF<o:p></o:p></b></p>
  <p class=TitArial1 style='text-autospace:ideograph-other'><span
  style='color:black'><o:p>&nbsp;</o:p></span></p>
  </td>
  <td width=96 valign=top style='width:72.05pt;border-top:none;border-left:
  none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>
  <p class=TitArial1 style='text-autospace:ideograph-other'>C&oacute;digo da
  Escola<span style='color:black'><o:p></o:p></span></p>
  </td>
  <td width=96 valign=top style='width:72.05pt;border-top:none;border-left:
  none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>
  <p class=TitArial1 style='text-autospace:ideograph-other'>N&uacute;mero de
  alunos<span style='color:black'><o:p></o:p></span></p>
  </td>
  <td width=96 valign=top style='width:72.05pt;border-top:none;border-left:
  none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>
  <p class=TitArial1 style='text-autospace:ideograph-other'>Nome da Escola<span
  style='color:black'><o:p></o:p></span></p>
  </td>
 </tr>
 <tr style='mso-yfti-irow:2;mso-yfti-lastrow:yes'>
  <td width=100 valign=top style='width:75.3pt;border:solid windowtext 1.0pt;
  border-top:none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 5.4pt 0cm 5.4pt'>
  <p class=TitArial1 style='text-autospace:ideograph-other'><b><span
  style='color:black'><o:p>&nbsp;</o:p></span></b></p>
  </td>
  <td width=96 valign=top style='width:72.0pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>
  <p class=TitArial1 style='text-autospace:ideograph-other'><b><span
  style='color:black'><o:p>&nbsp;</o:p></span></b></p>
  </td>
  <td width=96 valign=top style='width:72.05pt;border-top:none;border-left:
  none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>
  <p class=TitArial1 style='text-autospace:ideograph-other'><b><span
  style='color:black'><o:p>&nbsp;</o:p></span></b></p>
  </td>
  <td width=96 valign=top style='width:72.05pt;border-top:none;border-left:
  none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>
  <p class=TitArial1 style='text-autospace:ideograph-other'><b><span
  style='color:black'><o:p>&nbsp;</o:p></span></b></p>
  </td>
  <td width=96 valign=top style='width:72.05pt;border-top:none;border-left:
  none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>
  <p class=TitArial1 style='text-autospace:ideograph-other'><b><span
  style='color:black'><o:p>&nbsp;</o:p></span></b></p>
  </td>
  <td width=96 valign=top style='width:72.05pt;border-top:none;border-left:
  none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>
  <p class=TitArial1 style='text-autospace:ideograph-other'><b><span
  style='color:black'><o:p>&nbsp;</o:p></span></b></p>
  </td>
 </tr>
</table>
 
<p class=TitArial1 style='text-autospace:ideograph-other'><b><span
style='color:black'><o:p>&nbsp;</o:p></span></b></p>
 
<p class=TitArial1 style='text-autospace:ideograph-other'><b><span
style='color:black'><o:p>&nbsp;</o:p></span></b></p>
 
<p class=TitArial1 style='text-autospace:ideograph-other'><b><span
style='color:black'><o:p>&nbsp;</o:p></span></b></p>
 
<table class=MsoNormalTable border=1 cellspacing=0 cellpadding=0 width=657
 style='width:493.0pt;border-collapse:collapse;border:none;mso-border-alt:solid windowtext .5pt;
 mso-yfti-tbllook:160;mso-padding-alt:0cm 3.5pt 0cm 3.5pt;mso-border-insideh:
 .5pt solid windowtext;mso-border-insidev:.5pt solid windowtext'>
 <tr style='mso-yfti-irow:0;mso-yfti-firstrow:yes;height:18.75pt'>
  <td width=657 colspan=4 style='width:493.0pt;border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:18.75pt'>
  <p class=MsoNormal align=center style='text-align:center'><b><span
  style='color:black'>QUADRO II: PLANO PSE DE METAS MUNICIPAIS<o:p></o:p></span></b></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:18.75pt;border:none' width=0 height=25></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:1;height:18.75pt'>
  <td width=657 colspan=4 style='width:493.0pt;border:solid windowtext 1.0pt;
  border-top:none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:18.75pt'>
  <p class=MsoNormal><b><span style='color:black'>Componente I:
  avalia&ccedil;&atilde;o cl&iacute;nica e psicossocial<o:p></o:p></span></b></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:18.75pt;border:none' width=0 height=25></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:2;height:12.75pt'>
  <td width=146 style='width:109.15pt;border:solid windowtext 1.0pt;border-top:
  none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:12.75pt'>
  <p class=MsoNormal align=center style='text-align:center'><b><span
  style='color:black'>LINHA DE A&Ccedil;&Atilde;O <o:p></o:p></span></b></p>
  </td>
  <td width=178 style='width:133.3pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:12.75pt'>
  <p class=MsoNormal align=center style='text-align:center'><b><span
  style='color:black'>A&Ccedil;&Atilde;O <o:p></o:p></span></b></p>
  </td>
  <td width=150 style='width:112.2pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:12.75pt'>
  <p class=MsoNormal align=center style='text-align:center'><b><span
  style='color:black'>PAR&Acirc;METRO ESSENCIAL<o:p></o:p></span></b></p>
  </td>
  <td width=184 style='width:138.35pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:12.75pt'>
  <p class=MsoNormal align=center style='text-align:center'><b><span
  style='color:black'>METAS MUNICIPAIS PACTUADAS<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></b></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:12.75pt;border:none' width=0 height=17></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:3;height:25.5pt'>
  <td width=146 style='width:109.15pt;border:solid windowtext 1.0pt;border-top:
  none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:25.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Avalia&ccedil;&atilde;o Antropom&eacute;trica<o:p></o:p></span></p>
  </td>
  <td width=178 style='width:133.3pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:25.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Realizar avalia&ccedil;&atilde;o antropom&eacute;trica<o:p></o:p></span></p>
  </td>
  <td width=150 style='width:112.2pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:25.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <td width=184 style='width:138.35pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:25.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:25.5pt;border:none' width=0 height=34></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:4;height:14.6pt'>
  <td width=146 rowspan=2 style='width:109.15pt;border:solid windowtext 1.0pt;
  border-top:none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:14.6pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Atualiza&ccedil;&atilde;o do Calend&aacute;rio Vacinal<o:p></o:p></span></p>
  </td>
  <td width=178 rowspan=2 style='width:133.3pt;border-top:none;border-left:
  none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:14.6pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Realizar a verifica&ccedil;&atilde;o da carteira de
  vacina&ccedil;&atilde;o dos escolares<o:p></o:p></span></p>
  </td>
  <td width=150 rowspan=2 style='width:112.2pt;border-top:none;border-left:
  none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:14.6pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <td width=184 rowspan=2 style='width:138.35pt;border-top:none;border-left:
  none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:14.6pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:14.6pt;border:none' width=0 height=19></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:5;height:14.6pt'>
  <![if !supportMisalignedRows]>
  <td style='height:14.6pt;border:none' width=0 height=19></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:6;height:38.25pt'>
  <td width=146 style='width:109.15pt;border:solid windowtext 1.0pt;border-top:
  none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:38.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Detec&ccedil;&atilde;o precoce de Hipertens&atilde;o
  Arterial Sist&ecirc;mica (HAS)<o:p></o:p></span></p>
  </td>
  <td width=178 style='width:133.3pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:38.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Aferir a press&atilde;o arterial e identificar os
  escolares com HAS<o:p></o:p></span></p>
  </td>
  <td width=150 style='width:112.2pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:38.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <td width=184 style='width:138.35pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:38.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:38.25pt;border:none' width=0 height=51></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:7;height:27.0pt'>
  <td width=146 style='width:109.15pt;border:solid windowtext 1.0pt;border-top:
  none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:27.0pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Detec&ccedil;&atilde;o precoce de Agravos de Sa&uacute;de
  Negligenciados (prevalentes na regi&atilde;o: hansen&iacute;ase, tuberculose,
  mal&aacute;ria, etc...)<o:p></o:p></span></p>
  </td>
  <td width=178 style='width:133.3pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:27.0pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Realizar avalia&ccedil;&atilde;o clinica para identificar
  sinais de agravos de sa&uacute;de negligenciados observando indicadores
  epidemiol&oacute;gicos locais<o:p></o:p></span></p>
  </td>
  <td width=150 style='width:112.2pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:27.0pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <td width=184 style='width:138.35pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:27.0pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:27.0pt;border:none' width=0 height=36></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:8;height:27.0pt'>
  <td width=146 rowspan=2 style='width:109.15pt;border:solid windowtext 1.0pt;
  border-top:none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:27.0pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Avalia&ccedil;&atilde;o Oftalmol&oacute;gica <o:p></o:p></span></p>
  </td>
  <td width=178 rowspan=2 style='width:133.3pt;border-top:none;border-left:
  none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:27.0pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Realizar a triagem de acuidade visual dos escolares
  (Teste de Snellen) e identificar escolares problemas visuais<o:p></o:p></span></p>
  </td>
  <td width=150 rowspan=2 style='width:112.2pt;border-top:none;border-left:
  none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:27.0pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <td width=184 rowspan=2 style='width:138.35pt;border-top:none;border-left:
  none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:27.0pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:27.0pt;border:none' width=0 height=36></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:9;height:14.6pt'>
  <![if !supportMisalignedRows]>
  <td style='height:14.6pt;border:none' width=0 height=19></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:10;height:38.25pt'>
  <td width=146 style='width:109.15pt;border:solid windowtext 1.0pt;border-top:
  none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:38.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Avalia&ccedil;&atilde;o Auditiva<o:p></o:p></span></p>
  </td>
  <td width=178 style='width:133.3pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:38.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Avaliar e identificar os escolares com dificuldade de
  audi&ccedil;&atilde;o <o:p></o:p></span></p>
  </td>
  <td width=150 style='width:112.2pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:38.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&Atilde;O H&Aacute;<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <td width=184 style='width:138.35pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:38.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:38.25pt;border:none' width=0 height=51></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:11;height:38.25pt'>
  <td width=146 style='width:109.15pt;border:solid windowtext 1.0pt;border-top:
  none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:38.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Avalia&ccedil;&atilde;o Nutricional<o:p></o:p></span></p>
  </td>
  <td width=178 style='width:133.3pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:38.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Avaliar o estado nutricional e de h&aacute;bitos
  alimentares dos escolares.<o:p></o:p></span></p>
  </td>
  <td width=150 style='width:112.2pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:38.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <td width=184 style='width:138.35pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:38.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:38.25pt;border:none' width=0 height=51></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:12;height:63.75pt'>
  <td width=146 style='width:109.15pt;border:solid windowtext 1.0pt;border-top:
  none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:63.75pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Avalia&ccedil;&atilde;o da Sa&uacute;de Bucal<o:p></o:p></span></p>
  </td>
  <td width=178 style='width:133.3pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:63.75pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Avaliar o estado de sa&uacute;de bucal dos escolares e
  identificar os escolares com necessidade de cuidado em sa&uacute;de bucal<o:p></o:p></span></p>
  </td>
  <td width=150 style='width:112.2pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:63.75pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <td width=184 style='width:138.35pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:63.75pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:63.75pt;border:none' width=0 height=85></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:13;height:25.5pt'>
  <td width=146 rowspan=2 style='width:109.15pt;border:solid windowtext 1.0pt;
  border-top:none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:25.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Avalia&ccedil;&atilde;o Psicossocial<o:p></o:p></span></p>
  </td>
  <td width=178 rowspan=2 style='width:133.3pt;border-top:none;border-left:
  none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:25.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Verificar escolares sem registro civil e encaminhar<span
  style='mso-spacerun:yes'>&nbsp; </span>a informa&ccedil;&atilde;o/dados dos
  escolares ao Conselho Tutelar<span style='mso-spacerun:yes'>&nbsp; </span><o:p></o:p></span></p>
  </td>
  <td width=150 style='width:112.2pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:25.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <td width=184 style='width:138.35pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:25.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:25.5pt;border:none' width=0 height=34></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:14;height:25.5pt'>
  <td width=150 style='width:112.2pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:25.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <td width=184 style='width:138.35pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:25.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:25.5pt;border:none' width=0 height=34></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:15;height:12.75pt'>
  <td width=657 nowrap colspan=4 style='width:493.0pt;border:solid windowtext 1.0pt;
  border-top:none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:12.75pt'>
  <p class=MsoNormal><b><span style='color:black'>Componente II:
  a&ccedil;&otilde;es de promo&ccedil;&atilde;o da sa&uacute;de e
  preven&ccedil;&atilde;o <o:p></o:p></span></b></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:12.75pt;border:none' width=0 height=17></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:16;height:12.75pt'>
  <td width=146 style='width:109.15pt;border:solid windowtext 1.0pt;border-top:
  none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:12.75pt'>
  <p class=MsoNormal align=center style='text-align:center'><b><span
  style='color:black'>LINHA DE A&Ccedil;&Atilde;O <o:p></o:p></span></b></p>
  </td>
  <td width=178 style='width:133.3pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:12.75pt'>
  <p class=MsoNormal align=center style='text-align:center'><b><span
  style='color:black'>A&Ccedil;&Atilde;O <o:p></o:p></span></b></p>
  </td>
  <td width=150 style='width:112.2pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:12.75pt'>
  <p class=MsoNormal align=center style='text-align:center'><b><span
  style='color:black'>PAR&Acirc;METRO ESSENCIAL<o:p></o:p></span></b></p>
  </td>
  <td width=184 style='width:138.35pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:12.75pt'>
  <p class=MsoNormal align=center style='text-align:center'><b><span
  style='color:black'>METAS MUNICIPAIS PACTUADAS<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></b></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:12.75pt;border:none' width=0 height=17></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:17;height:89.25pt'>
  <td width=146 style='width:109.15pt;border:solid windowtext 1.0pt;border-top:
  none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:89.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>A&ccedil;&otilde;es de Seguran&ccedil;a Alimentar e
  Promo&ccedil;&atilde;o da Alimenta&ccedil;&atilde;o Saud&aacute;vel<o:p></o:p></span></p>
  </td>
  <td width=178 style='width:133.3pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:89.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Realizar atividades educativas sobre
  promo&ccedil;&atilde;o da alimenta&ccedil;&atilde;o e modos de vida
  saud&aacute;veis com a comunidade escolar, considerando os alimentos
  regionais.<o:p></o:p></span></p>
  </td>
  <td width=150 style='width:112.2pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:89.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <td width=184 style='width:138.35pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:89.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:89.25pt;border:none' width=0 height=119></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:18;height:51.0pt'>
  <td width=146 style='width:109.15pt;border:solid windowtext 1.0pt;border-top:
  none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:51.0pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Promo&ccedil;&atilde;o das Pr&aacute;ticas Corporais e
  Atividade F&iacute;sica nas Escolas<o:p></o:p></span></p>
  </td>
  <td width=178 style='width:133.3pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:51.0pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Realizar pr&aacute;ticas corporais orientadas,
  relacionadas &agrave; realidade da comunidade, inclu&iacute;das no cotidiano
  escolar<o:p></o:p></span></p>
  </td>
  <td width=150 style='width:112.2pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:51.0pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <td width=184 style='width:138.35pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:51.0pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:51.0pt;border:none' width=0 height=68></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:19;height:76.5pt'>
  <td width=146 rowspan=2 style='width:109.15pt;border:solid windowtext 1.0pt;
  border-top:none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:76.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Sa&uacute;de e Preven&ccedil;&atilde;o nas Escolas
  (SPE):Educa&ccedil;&atilde;o para a Sa&uacute;de Sexual, Sa&uacute;de
  Reprodutiva e preven&ccedil;&atilde;o das DST/AIDS &#8211; Sa&uacute;de e
  Preven&ccedil;&atilde;o nas Escolas<o:p></o:p></span></p>
  </td>
  <td width=178 style='width:133.3pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:76.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Realizar atividades abordando as tem&aacute;ticas da
  sa&uacute;de sexual, sa&uacute;de reprodutiva e preven&ccedil;&atilde;o das
  DST/AIDS e Hepatites Virais no cotidiano da escola<o:p></o:p></span></p>
  </td>
  <td width=150 style='width:112.2pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:76.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <td width=184 style='width:138.35pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:76.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:76.5pt;border:none' width=0 height=102></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:20;height:89.25pt'>
  <td width=178 style='width:133.3pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:89.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Formar jovens multiplicadores para atuarem entre pares
  nas tem&aacute;ticas envolvendo sa&uacute;de sexual, sa&uacute;de reprodutiva
  e preven&ccedil;&atilde;o das DST/AIDS e Hepatites Virais<o:p></o:p></span></p>
  </td>
  <td width=150 style='width:112.2pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:89.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <td width=184 style='width:138.35pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:89.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:89.25pt;border:none' width=0 height=119></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:21;height:63.75pt'>
  <td width=146 style='width:109.15pt;border:solid windowtext 1.0pt;border-top:
  none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:63.75pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Sa&uacute;de e Preven&ccedil;&atilde;o nas Escolas (SPE):
  Preven&ccedil;&atilde;o ao uso de &Aacute;lcool e Tabaco e outras Drogas<o:p></o:p></span></p>
  </td>
  <td width=178 style='width:133.3pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:63.75pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Realizar atividades abordando a tem&aacute;tica dos
  riscos e danos do uso de &aacute;lcool, tabaco, crack e outras drogas no
  cotidiano da escola<o:p></o:p></span></p>
  </td>
  <td width=150 style='width:112.2pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:63.75pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <td width=184 style='width:138.35pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:63.75pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:63.75pt;border:none' width=0 height=85></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:22;height:76.5pt'>
  <td width=146 style='width:109.15pt;border:solid windowtext 1.0pt;border-top:
  none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:76.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Promo&ccedil;&atilde;o da Cultura de Paz e
  Preven&ccedil;&atilde;o das Viol&ecirc;ncias<o:p></o:p></span></p>
  </td>
  <td width=178 style='width:133.3pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:76.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Realizar atividades abordando as tem&aacute;ticas da
  diversidade sexual , Bullyng, homofobia, discrimina&ccedil;&atilde;o e
  preconceito no cotidiano da escola<o:p></o:p></span></p>
  </td>
  <td width=150 style='width:112.2pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:76.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <td width=184 style='width:138.35pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:76.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:76.5pt;border:none' width=0 height=102></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:23;height:76.5pt'>
  <td width=146 style='width:109.15pt;border:solid windowtext 1.0pt;border-top:
  none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:76.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Promo&ccedil;&atilde;o da sa&uacute;de ambiental e
  desenvolvimento sustent&aacute;vel<o:p></o:p></span></p>
  </td>
  <td width=178 style='width:133.3pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:76.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Realizar atividades de sensibiliza&ccedil;&atilde;o,
  responsabiliza&ccedil;&atilde;o e interven&ccedil;&atilde;o do cuidado
  consigo mesmo e com o ambiente escolar<o:p></o:p></span></p>
  </td>
  <td width=150 style='width:112.2pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:76.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <td width=184 style='width:138.35pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:76.5pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de escolares a serem contemplados<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:76.5pt;border:none' width=0 height=102></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:24;height:12.75pt'>
  <td width=657 nowrap colspan=4 style='width:493.0pt;border:solid windowtext 1.0pt;
  border-top:none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:12.75pt'>
  <p class=MsoNormal><b><span style='color:black'>Componente III:
  educa&ccedil;&atilde;o permanente e capacita&ccedil;&atilde;o de
  profissionais da educa&ccedil;&atilde;o e da sa&uacute;de e de jovens para o
  PSE <o:p></o:p></span></b></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:12.75pt;border:none' width=0 height=17></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:25;height:12.75pt'>
  <td width=146 style='width:109.15pt;border:solid windowtext 1.0pt;border-top:
  none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:12.75pt'>
  <p class=MsoNormal align=center style='text-align:center'><b><span
  style='color:black'>LINHA DE A&Ccedil;&Atilde;O <o:p></o:p></span></b></p>
  </td>
  <td width=178 style='width:133.3pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:12.75pt'>
  <p class=MsoNormal align=center style='text-align:center'><b><span
  style='color:black'>A&Ccedil;&Atilde;O <o:p></o:p></span></b></p>
  </td>
  <td width=150 style='width:112.2pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:12.75pt'>
  <p class=MsoNormal align=center style='text-align:center'><b><span
  style='color:black'>PAR&Acirc;METRO ESSENCIAL<o:p></o:p></span></b></p>
  </td>
  <td width=184 style='width:138.35pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:12.75pt'>
  <p class=MsoNormal align=center style='text-align:center'><b><span
  style='color:black'>METAS MUNICIPAIS PACTUADAS<span
  style='mso-spacerun:yes'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><o:p></o:p></span></b></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:12.75pt;border:none' width=0 height=17></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:26;height:38.25pt'>
  <td width=146 rowspan=3 style='width:109.15pt;border:solid windowtext 1.0pt;
  border-top:none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 3.5pt 0cm 3.5pt;height:38.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Educa&ccedil;&atilde;o permanente e
  capacita&ccedil;&atilde;o local de profissionais da educa&ccedil;&atilde;o
  nos temas da sa&uacute;de e constitui&ccedil;&atilde;o das equipes de
  sa&uacute;de que atuar&atilde;o nos Territ&oacute;rios do Programa Sa&uacute;de
  na Escola<o:p></o:p></span></p>
  </td>
  <td width=178 style='width:133.3pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:38.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Capacitar os profissionais da sa&uacute;de e
  educa&ccedil;&atilde;o para gest&atilde;o inter-setorial do PSE<o:p></o:p></span></p>
  </td>
  <td width=150 rowspan=3 style='width:112.2pt;border-top:none;border-left:
  none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:38.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&Atilde;O H&Aacute;<o:p></o:p></span></p>
  </td>
  <td width=184 style='width:138.35pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:38.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de profissionais a serem contemplados <o:p></o:p></span></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:38.25pt;border:none' width=0 height=51></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:27;height:51.0pt'>
  <td width=178 style='width:133.3pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:51.0pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Capacitar os profissionais da sa&uacute;de e
  educa&ccedil;&atilde;o para aplica&ccedil;&atilde;o do Teste de Snellen
  (acuidade visual)<o:p></o:p></span></p>
  </td>
  <td width=184 style='width:138.35pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:51.0pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de profissionais a serem contemplados <o:p></o:p></span></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:51.0pt;border:none' width=0 height=68></td>
  <![endif]>
 </tr>
 <tr style='mso-yfti-irow:28;mso-yfti-lastrow:yes;height:140.25pt'>
  <td width=178 style='width:133.3pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:140.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>Capacitar os profissionais de sa&uacute;de e
  educa&ccedil;&atilde;o para trabalhar com as tem&aacute;ticas:
  Educa&ccedil;&atilde;o para a Sa&uacute;de Sexual, Sa&uacute;de Reprodutiva e
  preven&ccedil;&atilde;o das DST/AIDS &#8211; Sa&uacute;de e Preven&ccedil;&atilde;o
  nas Escolas;<span style='mso-spacerun:yes'>&nbsp;
  </span>Promo&ccedil;&atilde;o da Alimenta&ccedil;&atilde;o Saud&aacute;vel;
  Preven&ccedil;&atilde;o ao uso de &Aacute;lcool e Tabaco, Crack e outras
  Drogas<o:p></o:p></span></p>
  </td>
  <td width=184 style='width:138.35pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt;height:140.25pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='color:black'>N&ordm; de profissionais a serem contemplados <o:p></o:p></span></p>
  </td>
  <![if !supportMisalignedRows]>
  <td style='height:140.25pt;border:none' width=0 height=187></td>
  <![endif]>
 </tr>
 
 -->
 
 <?php
	if($_SESSION['conid']){
		$sql = "SELECT  m.mpaid,
						m.mpaacao06comp1,
						m.mpaacao01comp3,
						m.mpaacao02comp3,
						m.mpaacao03comp3,
						m.mpaacao04comp3,
						m.mpaacao05comp3
			     FROM 
			     		pse.metapactuada m
			     WHERE 
			     		m.conid = {$_SESSION['conid']}";
		$dados = $db->carregar($sql);
		if($dados) extract($dados[0]);
		
		$sql = "SELECT sum(pamquanteduseratend) as total FROM pse.planoacaoc1 
				WHERE conid = {$_SESSION['conid']}";
		$concoberturacomp1 = $db->pegaUm($sql);
		
		$sql = "SELECT sum(pacquanteduseratend) as total FROM pse.planoacaoc2 
				WHERE conid = {$_SESSION['conid']}";
		$concoberturacomp2 = $db->pegaUm($sql);
		
	}
 ?>
 
 
 <tr style='mso-yfti-irow:0;mso-yfti-firstrow:yes'>
  <td width=581 colspan=6 valign=top style='width:435.5pt;border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>
  <p class=TitArial1 style='text-autospace:ideograph-other'>COMPONENTE I - AVALIAÇÃO CLÍNICA E PSICOSOCIAL</b></span>
		 </center></td>
</tr>
<tr>
	<td width="45%" style='width:435.5pt;border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt' >LINHA DE AÇÃO</td>
	<td width="45%" style='width:435.5pt;border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>AÇÃO</td>
	<td width="10%" style='width:435.5pt;border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>META PACTUADA</td>
</tr>
<?php 
	$sql = "select laaid, laalinhaacao, laaacao, laacomponente, laastatus
			from pse.complinhaacao 
			where laacomponente = '1'
			and laastatus = 'A' ";
	$dados = $db->carregar($sql);
	
	foreach($dados as $d){
		?>
		<tr>
			<td width="45%" style='width:435.5pt;border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><?=$d['laalinhaacao']?></td>
			<td width="45%" style='border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><?=$d['laaacao']?></td>
			<td width="10%" style='border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt' nowrap="nowrap" >
				<?
					if($d['laaid'] == '6'){
						echo $mpaacao06comp1;
					}else{
						echo $concoberturacomp1;
					}
				?>
			</td>
		</tr>
	<?}?>

</table>
 
<p class=TitArial1 style='text-autospace:ideograph-other'><b><span
style='color:black'><o:p>&nbsp;</o:p></span></b></p>
 
<p class=TitArial1 style='text-autospace:ideograph-other'><b><span
style='color:black'><o:p>&nbsp;</o:p></span></b></p>
 
<p class=TitArial1 style='text-autospace:ideograph-other'><b><span
style='color:black'><o:p>&nbsp;</o:p></span></b></p>
 
<table class=MsoNormalTable border=1 cellspacing=0 cellpadding=0
 style='border-collapse:collapse;border:none;mso-border-alt:solid windowtext .5pt;
 mso-yfti-tbllook:160;mso-padding-alt:0cm 5.4pt 0cm 5.4pt;mso-border-insideh:
 .5pt solid windowtext;mso-border-insidev:.5pt solid windowtext'>

<tr>
	<td width=581 colspan=6 valign=top style='border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>
  <p class=TitArial1 style='text-autospace:ideograph-other'>COMPONENTE II - PROMOÇÃO E PREVENÇÃO À SAÚDE</td>
</tr>
<tr>
	<td width="45%" style='width:435.5pt;border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>LINHA DE AÇÃO</td>
	<td width="45%" style='width:435.5pt;border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>AÇÃO</td>
	<td width="10%" style='width:435.5pt;border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>META PACTUADA</td>
</tr>
<?php 
	$sql = "select laaid, laalinhaacao, laaacao, laacomponente, laastatus
			from pse.complinhaacao 
			where laacomponente = '2'
			and laastatus = 'A' ";
	$dados = $db->carregar($sql);
	$i = 1;
	foreach($dados as $d){
		?>
		<tr>
			<?if($i==3){?>
				<td width="45%" style='width:435.5pt;border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt' rowspan=2><?=$d['laalinhaacao']?></td>
			<?}elseif($i!=3 && $i!=4){?>
				<td width="45%" style='width:435.5pt;border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt' ><?=$d['laalinhaacao']?></td>
			<?}?>
			<td width="45%" style='border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><?=$d['laaacao']?></td>
			<td width="10%" style='border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt' nowrap="nowrap" >
  			<?php if( $d['laaid'] == '13' ): ?>
  				<?php echo $mpaacao04comp3; ?>
  			<?php else: ?>
				<? echo $concoberturacomp2; ?>
			<?php endif; ?>
			</td>
		</tr>
		<?
		$i++;
	}
	?>

</table>
 
<p class=TitArial1 style='text-autospace:ideograph-other'><b><span
style='color:black'><o:p>&nbsp;</o:p></span></b></p>
 
<p class=TitArial1 style='text-autospace:ideograph-other'><b><span
style='color:black'><o:p>&nbsp;</o:p></span></b></p>
 
<p class=TitArial1 style='text-autospace:ideograph-other'><b><span
style='color:black'><o:p>&nbsp;</o:p></span></b></p>
 
<table class=MsoNormalTable border=1 cellspacing=0 cellpadding=0
 style='border-collapse:collapse;border:none;mso-border-alt:solid windowtext .5pt;
 mso-yfti-tbllook:160;mso-padding-alt:0cm 5.4pt 0cm 5.4pt;mso-border-insideh:
 .5pt solid windowtext;mso-border-insidev:.5pt solid windowtext'>



<tr>
<td width=581 colspan=6 valign=top style='width:435.5pt;border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>
  <p class=TitArial1 style='text-autospace:ideograph-other'>
	COMPONENTE III - EDUCAÇÃO PERMANENTE</td>
</tr>
<tr>
	<td width="45%" style='width:435.5pt;border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>LINHA DE AÇÃO</td>
	<td width="45%" style='border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>AÇÃO</td>
	<td width="10%" style='border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>META PACTUADA</td>
</tr>
<?php 
	$sql = "select laaid, laalinhaacao, laaacao, laacomponente, laastatus
			from pse.complinhaacao 
			where laacomponente = '3'
			and laastatus = 'A' ";
	$dados = $db->carregar($sql);
	$i = 1; 
	foreach($dados as $d){
		?>
		<tr>
			<?if($i==1){?>
				<td width="45%" style='width:435.5pt;border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt' rowspan=3><?=$d['laalinhaacao']?></td>
			<?}elseif($i>3){?>
				<td width="45%" style='width:435.5pt;border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt' ><?=$d['laalinhaacao']?></td>
			<?}?>
			<td width="45%" style='border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><?=$d['laaacao']?></td>
			<td width="10%" style='border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt' nowrap="nowrap" >
				<?
					echo ${'mpaacao0'.$i.'comp3'};  //campo_texto( 'mpaacao0'.$i.'comp3', 'S', 'S', '', 18, 6, '######', '', 'right', '', 0, '', '', '', '');
				?>
			</td>
		</tr>
		<?
		$i++;
	}
	?>
 
 
 
 
 
</table>
 
<p class=MsoNormal><o:p>&nbsp;</o:p></p>
 
<p class=MsoNormal><o:p>&nbsp;</o:p></p>
 
<p class=MsoNormal><o:p>&nbsp;</o:p></p>
 
<p class=MsoNormal><b><?=$municipio?></b>, <b><?=date("d")?></b> de <b><?=mes_extenso(date("m"))?></b> de <b><?=date("Y")?></b></p>
 
<p class=MsoNormal><o:p>&nbsp;</o:p></p>
 
<p class=MsoNormal><o:p>&nbsp;</o:p></p>
<p class=MsoNormal><o:p>&nbsp;</o:p></p>
<p class=MsoNormal><o:p>&nbsp;</o:p></p>
 
<table width="90%">
	<tr>
		<td>
			<p class=MsoNormal>___________________________________</p>
			<b><?=strtoupper($consenomesecretario)?></b>
			<br> Secret&aacute;rio(a) Municipal de Educa&ccedil;&atilde;o
			<br><br><br><br>Carimbo
		</td>
		<td>
			<p class=MsoNormal>___________________________________</p>
			<b><?=strtoupper($conssnomesecretario)?></b>
			<br> Secret&aacute;rio(a) Municipal de Sa&uacute;de
			<br><br><br><br>Carimbo
		</td>
	</tr> 
</table>
 

</div>
 
</body>
 
</html>


	<?
	
}

?>
