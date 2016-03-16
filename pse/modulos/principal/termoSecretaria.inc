<?php
include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
$muncod = $_SESSION['pse']['muncod'];
//echo $muncod;
$sql ="	SELECT a.empid,c.mundescricao, a.semnomesecretarioeducacao, d.muncodcapital
		FROM pse.secretariaestmun a
		INNER JOIN
			pse.estadomunicipiopse b ON b.empid = a.empid	
		INNER JOIN
			territorios.municipio c ON c.muncod = b.muncod
		LEFT JOIN
			territorios.estado d ON d.muncodcapital = b.muncod
		WHERE c.muncod = '$muncod' AND a.semsecretariaestadualmunicipal='M'";
$dados = $db->pegalinha($sql);

$govmunicipal		= ($dados['mundescricao']==''?'______________________________________':$dados['mundescricao']);
$nomesec			= ($dados['semnomesecretarioeducacao']==''?'______________________________________':$dados['semnomesecretarioeducacao']);
$capital			= ($dados['muncodcapital']==''?0:$dados['muncodcapital']);
$cnpj				= "o nº ____________________";
$paragrafo = "O Secretário de Educação indica, abaixo, o nome de seu respectivo representante 
interlocutor do PSE e encaminha este Termo de Adesão ao Ministério da Educação, após o 
preenchimento do Cadastro Estado/Município, no referido sítio eletrônico. E, por estar assim de acordo com as disposições 
deste, o Secretário Municipal de Educação firma o presente Termo de Adesão ao PSE. ";

$assinatura1 = "______________________________<br>$nomesec<br>Secretário Municipal de Educação";

if($capital>0){
	$sql ="	SELECT semnomesecretarioeducacao 
			FROM pse.secretariaestmun
			WHERE empid = $dados[empid]";
	$dados = $db->pegalinha($sql);

	$nomesec = ($dados['semnomesecretarioeducacao']==''?'________________________':$dados['semnomesecretarioeducacao']);
	$secestadual = " e da <b>Secretaria Estadual de Educação</b> ";
	$cnpj = "os nºs ____________________ e ____________________, respectivamente";
	$paragrafo = "Os Secretários de Educação indicam, abaixo, o nome de seus respectivos representantes 
	interlocutores do PSE e encaminham este Termo de Adesão ao Ministério da Educação, após o 
	preenchimento do Cadastro Estado/Município, no referido sítio eletrônico. E, por estarem assim de acordo com as disposições 
	deste, o Secretário Municipal de Educação e o Secretário Estadual de Educação firmam o presente Termo de Adesão ao PSE.";
	$assinatura2 = "<td align='center'><b>______________________________<br>$nomesec<br>Secretário Estadual de Educação</b></td>";
}


$secretario		= ($capital==0?'Municipal':'Estadual');
$estadocivil	= '_______________';
$rg				= '_______________';
$exprg			= '_______________';
$cpf			= '_______________';
		
		
		
?>
<STYLE TYPE="text/css">
.quebra {
    page-break-after: always;
}
</STYLE>
<p align="right"><font size="1"><a href="#" onclick="self.print();">Imprimir</a></font></p>
<body onload="self.print();">
<table align="center" cellspacing="1" cellpadding="4">
	<tr>
		<td align="center"><img src="../imagens/brasao.gif" border="0" width="60" height="60"></td>
	</tr>
	<tr>
		<td align="center">
			<b>Ministério da Educação<br>
			Secretaria de Educação Continuada, Alfabetização e Diversidade<br>
			Diretoria de educação Integral, Direitos Humanos e Cidadania</b>
		</td>
	</tr>
	<tr>
		<td align="center">
			<br><br>
			<b>TERMO DE ADESÂO</b>
			<br><br>
		</td>
	</tr>
</table>


<p align="justify">

O <b>Governo Municipal de <?=$govmunicipal;?></b>, por intermédio de sua <b>Secretaria Municipal de Educação</b> 
<?=$secestadual;?>, inscritas no CNPJ sob <?=$cnpj;?>, 
neste ato representado por seu Secretário <?=$secretario;?> de Educação, <b><?=$nomesec;?></b>, <?=$estadocivil;?>, portador da 
carteira de identidade nº <?=$rg;?>, expedida por <?=$exprg;?>, e inscritos no CPF sob o 
nº. <?=$cpf;?>, considerando o que dispõe a Constituição Federal, as Leis nº. 8.080/90 e nº. 8.142/90 
da Saúde e a Lei nº. 9.394/96 da Educação celebra o presente Termo de Adesão ao Programa Saúde na 
Escola, que se efetivará por meio de gestão intersetorial entre Educação e Saúde no âmbito federal, 
estadual, municipal e do Distrito Federal.
<br><br>
As responsabilidades da gestão municipal assumidas por meio deste Termo de Adesão estão explícitas no 
Projeto do PSE Municipal e no Cadastro Estado/Município, este localizado no sítio eletrônico http://simec.mec.gov.br. 
<br><br>
<?=$paragrafo;?>
<br><br>
<center>
<?=$govmunicipal;?>, _____ de _____________________ de 20___.
<br><br><br><br>
<table border='0' width="100%">
	<tr>
		<td align='center'><b><?=$assinatura1;?></b></td>
		<?=$assinatura2;?>
	</tr>
</table>
</center>
<br><br>
<table border='0' width="100%">
	<tr>
		<td colspan="2"><b><u>Representante(s):</u></b></td>
	</tr>	
	<tr>
		<td>
			<b>Secretaria Municipal de Educação</b><br>
			Nome:<br>
			e-mail:<br>
			Telefone(s):
		</td>	
		<?php if($capital>0){?>
		<td>
		<b>Secretaria Estadual de Educação</b><br>
		Nome:<br>
		e-mail:<br>
		Telefone(s):
		</td>
		<?php }?>
	</tr>
</table>	
</p>    


</body>