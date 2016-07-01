<?php

global $db;
if($_REQUEST['pjuid']) $_SESSION['projovemurbano']['pjuid']=$_REQUEST['pjuid'];
if(!$_REQUEST['caeid']){
	echo '<script>
				alert("Comprovante Inexistente! Tente novamente.");
				window.close();
		  </script>';
} 

$sql = "select cae.caenome, cae.caecpf, cae.caenomemae, 
				to_char(cae.caedatanasc::date,'DD/MM/YYYY') as caedatanasc, 
				cae.caenumrg || ' ' || cae.caeorgaoexpedidorrg as rg, 
				cae.caelogradouro || ' nº: ' || cae.caenumero as endaluno,
				ent.entnumcpfcnpj, 
				ent.entnome as nomeescola,
				ende.endlog || ' nº: ' || ende.endnum as endescola,
				caeano||lpad(cae.caeid::varchar,6,'0') as ninscricao,
				turdesc as turma,
				mun.mundescricao as municipioescola,
				mun2.mundescricao as municipioaluno,
				est.estdescricao as estado,
				CASE 
					WHEN pju.estuf IS NOT NULL THEN 'E'
					WHEN pju.muncod IS NOT NULL THEN 'M'
				END as esfera
		from projovemurbano.cadastroestudante cae
		inner join projovemurbano.turma tur ON tur.turid = cae.turid 
		LEFT join projovemurbano.nucleoescola nuc ON nuc.nucid = cae.nucid AND nuc.entid = tur.entid 
		inner join entidade.entidade ent ON ent.entid = tur.entid
		inner join entidade.endereco ende ON ende.entid = ent.entid
		left join territorios.municipio mun ON mun.muncod = ende.muncod 
		left join territorios.municipio mun2 ON mun2.muncod = cae.muncod::varchar 
		LEFT JOIN projovemurbano.projovemurbano pju ON pju.pjuid = cae.pjuid
		
		left join territorios.estado est ON est.estuf = ende.estuf
		where cae.caeid = ".$_REQUEST['caeid'];
// ver($sql,d);
$dados = $db->pegaLinha($sql);
if($dados) extract($dados);
 
?>

<html>
<body style="font-size:7pt;" leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">


<script language="JavaScript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>

<form name="formulario" action="" method="post">

<br>
<center>
	<a onclick="print();" style="font-size:14px;color:blue">[Imprimir]</a>
</center>
<br>
<table width="95%" cellSpacing="1" cellPadding="3" align="center" style="border-bottom: 1px solid black; border-left: 1px solid black; border-right: 1px solid black; border-top: 1px solid black;">
	<tr>
		<td align="center" >
			<b>COMPROVANTE DE MATRÍCULA</b>
			<br>
			(via do estudante)
		</td>
	</tr>
	<tr>
		<td>
			Prezado Jovem,
			
			<br><br>
			
			Sua matrícula foi efetivada no Programa Nacional de Inclusão de Jovens – Projovem Urbano no 
			<?=($municipioaluno ? 'Município' : 'Estado')?> 
			de <b><?=($municipioescola ? $municipioescola : $estado)?></b> 
			pela Secretaria <?=($esfera == 'M' ? 'Municipal' : 'Estadual')?> de Educação, 
			sob o nº <b><?=$ninscricao?></b> 
			na Escola <b><?=$nomeescola?></b>, 
			endereço <b><?=$endescola?></b> 
			para início das aulas em <b> <?if($_SESSION['projovemurbano']['ppuid']!=3){?>23 de setembro de 2013</b>.<?}ELSE{?>________________________________<?}?> 
			
			<br><br>
			
			Ressalta-se que:
			
			<br><br>
			
			<div style="padding-left:15px;">
				<li>O curso tem duração de 18 meses ininterruptos.</li>
				<li>Você deverá ter freqüência de 75% nas aulas dadas de cada mês e entregar 75% dos trabalhos pedagógicos do mesmo período, para receber seu auxílio financeiro no valor de R$ 100,00/mês (cem reais), não podendo ultrapassar 18 pagamentos no Programa.</li>
				<li>Portanto, caso você já tenha participado do Programa, e já tenha recebido algum auxílio financeiro, você receberá apenas os que faltam para completar os 18 auxílios financeiros.</li>
				<li>Ao final do curso você deverá ter freqüência de 75% nas atividades presenciais e 50% da pontuação distribuída, para ser certificado no Projovem Urbano (ensino fundamental/EJA, qualificação profissional e participação cidadã).</li>
			</div>
			
			<br><br>
			
			Confira abaixo os dados pessoais registrados na matrícula.
			
			<br><br>
			
			<div style="padding-left:15px;">
				
				<b>Número de Matrícula:</b> <?=$ninscricao?>
				<br><b>Turma:</b> <?=$turma?>
				<br><b>CPF:</b> <?=$caecpf?>
				<br><b>Nome:</b> <?=$caenome?>
				<br><b>Data de nascimento:</b> <?=$caedatanasc?>
				<br><b>Nome da mãe:</b> <?=$caenomemae?>
				<br><b>RG:</b> <?=$rg?>
				<br><b>Endereço:</b> <?=$endaluno?>
				
			</div>
			<br><br>
			Declaro que as informações prestadas no ato da matrícula e os dados pessoais informados estão corretos e são verídicos.
			Estou ciente das condições e informações relativas ao recebimento do auxílio financeiro do Projovem Urbano e demais informações contidas neste documento.
			
			<br><br><br><br><br><br><br><br>
			
			<div align="right">
			<b><?=($municipioescola ? $municipioescola : $estado)?>, <?=date("d")?> de <?=mes_extenso(date("m"))?> de 2015.</b>
			</div>
			
			<br><br><br><br>

			<div align="center">
			___________________________________________
			<br>Assinatura do estudante ou representante legal
			</div>
			<br>
		</td>
	</tr>
</table>

<br>

<div style='page-break-before: always'> </div>

<br>

<table width="95%" cellSpacing="1" cellPadding="3" align="center" style="border-bottom: 1px solid black; border-left: 1px solid black; border-right: 1px solid black; border-top: 1px solid black;">
	<tr>
		<td align="center" >
			<b>COMPROVANTE DE MATRÍCULA</b>
			<br>
			(via da escola)
		</td>
	</tr>
	<tr>
		<td>
			Prezado Jovem,
			
			<br><br>
			
			Sua matrícula foi efetivada no Programa Nacional de Inclusão de Jovens – Projovem Urbano no 
			<?=($municipioaluno ? 'Município' : 'Estado')?> 
			de <b><?=($municipioescola ? $municipioescola : $estado)?></b> 
			pela Secretaria <?=($municipioaluno ? 'Municipal' : 'Estadual')?> de Educação, 
			sob o nº <b><?=$ninscricao?></b> 
			na Escola <b><?=$nomeescola?></b>, 
			endereço <b><?=$endescola?></b> 
			para início das aulas em <b> <?if($_SESSION['projovemurbano']['ppuid']!=3){?>23 de setembro de 2013</b>.<?}ELSE{?>________________________________<?}?> 
			
			<br><br>
			
			Ressalta-se que:
			
			<br><br>
			
			<div style="padding-left:15px;">
				<li>O curso tem duração de 18 meses ininterruptos.</li>
				<li>Você deverá ter freqüência de 75% nas aulas dadas de cada mês e entregar 75% dos trabalhos pedagógicos do mesmo período, para receber seu auxílio financeiro no valor de R$ 100,00/mês (cem reais), não podendo ultrapassar 18 pagamentos no Programa.</li>
				<li>Portanto, caso você já tenha participado do Programa, e já tenha recebido algum auxílio financeiro, você receberá apenas os que faltam para completar os 18 auxílios financeiros.</li>
				<li>Ao final do curso você deverá ter freqüência de 75% nas atividades presenciais e 50% da pontuação distribuída, para ser certificado no Projovem Urbano (ensino fundamental/EJA, qualificação profissional e participação cidadã).</li>
			</div>
			
			<br><br>
			
			Confira abaixo os dados pessoais registrados na matrícula.
			
			<br><br>
			
			<div style="padding-left:15px;">
				
				<b>Número de Matrícula:</b> <?=$ninscricao?>
				<br><b>Turma:</b> <?=$turma?>
				<br><b>CPF:</b> <?=$caecpf?>
				<br><b>Nome:</b> <?=$caenome?>
				<br><b>Data de nascimento:</b> <?=$caedatanasc?>
				<br><b>Nome da mãe:</b> <?=$caenomemae?>
				<br><b>RG:</b> <?=$rg?>
				<br><b>Endereço:</b> <?=$endaluno?>
			</div>
						
			<br><br>

			Declaro que as informações prestadas no ato da matrícula e os dados pessoais informados estão corretos e são verídicos.
			Estou ciente das condições e informações relativas ao recebimento do auxílio financeiro do Projovem Urbano e demais informações contidas neste documento.
			
			<br><br><br><br><br><br><br><br>
			
			<div align="right">
			<b><?=($municipioescola ? $municipioescola : $estado)?>, <?=date("d")?> de <?=mes_extenso(date("m"))?> de 2015.</b>
			</div>
			
			<br><br><br><br>

			<div align="center">
			___________________________________________
			<br>Assinatura do estudante ou representante legal
			</div>
			<br>
		</td>
	</tr>
</table>

</form>
</body>
</html>

<script>
	window.print();
</script>