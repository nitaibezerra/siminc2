<?php

global $db;
if($_REQUEST['apcid']) $_SESSION['projovemurbano']['apcid']=$_REQUEST['apcid'];
if(!$_REQUEST['estid']){
	echo '<script>
				alert("Comprovante Inexistente! Tente novamente.");
				window.close();
		  </script>';
}
 
$sql = "SELECT 
			est.estnome, 
			est.estcpf, 
			est.estnomemae, 
			to_char(est.estdatanascimento::date,'DD/MM/YYYY') as estdatanascimento, 
			est.estnumrg || ' ' || est.estorgaoexpedidorg as rg, 
			est.estendlogradouro || ' nº: ' || est.estendnumero as endaluno,
			ent.entnumcpfcnpj, 
			ent.entnome as nomeescola,
			ende.endlog || ' nº: ' || ende.endnum as endescola,
			'2014'||lpad(est.estid::varchar,6,'0') as ninscricao,
			turdescricao as turma,
			mun.mundescricao as municipioescola,
			mun2.mundescricao as municipioaluno,
			esta.estdescricao as estado,
			CASE 
				WHEN apc.apcesfera = 'E' THEN 'E'
				ELSE 'M'
			END as esfera
		FROM
					projovemcampo.estudante est
		INNER JOIN projovemcampo.turma tur ON tur.turid = est.turid 
		INNER JOIN entidade.entidade ent ON ent.entid = tur.entid
		INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
		LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod 
		LEFT JOIN  territorios.municipio mun2 ON mun2.muncod = est.estendmuncod::varchar 
		LEFT JOIN projovemcampo.adesaoprojovemcampo apc ON apc.secaid = tur.secaid
		LEFT JOIN  territorios.estado esta ON esta.estuf = ende.estuf
		WHERE 
			est.estid ={$_REQUEST['estid']}";
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
			
			Sua matrícula foi efetivada no Programa Nacional de Inclusão de Jovens – Projovem Campo no 
			<?=($esfera == 'M' ? 'Município' : 'Estado')?> 
			de <b><?=($esfera == 'M' ? $municipioescola : $estado)?></b> 
			pela Secretaria <?=($esfera == 'M' ? 'Municipal' : 'Estadual')?> de Educação, 
			sob o nº <b><?=$ninscricao?></b> 
			na Escola <b><?=$nomeescola?></b>, 
			endereço <b><?=$endescola?></b> 
			para início das aulas em <b> ________________________________ .
			
			<br><br>
			
<!-- 			Ressalta-se que: -->
			
<!-- 			<br><br> -->
			
			<div style="padding-left:15px;">
<!-- 				<li>O curso tem duração de 18 meses ininterruptos.</li> -->
<!-- 				<li>Você deverá ter freqüência de 75% nas aulas dadas de cada mês e entregar 75% dos trabalhos pedagógicos do mesmo período, para receber seu auxílio financeiro no valor de R$ 100,00/mês (cem reais), não podendo ultrapassar 18 pagamentos no Programa.</li> -->
<!-- 				<li>Portanto, caso você já tenha participado do Programa, e já tenha recebido algum auxílio financeiro, você receberá apenas os que faltam para completar os 18 auxílios financeiros.</li> -->
<!-- 				<li>Ao final do curso você deverá ter freqüência de 75% nas atividades presenciais e 50% da pontuação distribuída, para ser certificado no Projovem Campo (ensino fundamental/EJA, qualificação profissional e participação cidadã).</li> -->
<!-- 			</div> -->
			
<!-- 			<br><br> -->
			
			Confira abaixo os dados pessoais registrados na matrícula.
			
			<br><br>
			
			<div style="padding-left:15px;">
				
				<b>Número de Matrícula:</b> <?=$ninscricao?>
				<br><b>Turma:</b> <?=$turma?>
				<br><b>CPF:</b> <?=$estcpf?>
				<br><b>Nome:</b> <?=$estnome?>
				<br><b>Data de nascimento:</b> <?=$estdatanascimento?>
				<br><b>Nome da mãe:</b> <?=$estnomemae?>
				<br><b>RG:</b> <?=$rg?>
				<br><b>Endereço:</b> <?=$endaluno?>
				
			</div>
						
			<br><br>

			Declaro que as informações prestadas no ato da matrícula e os dados pessoais informados estão corretos e são verídicos.
			Estou ciente das condições e informações relativas ao recebimento do auxílio financeiro do Projovem Campo e demais informações contidas neste documento.
			
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
			
			Sua matrícula foi efetivada no Programa Nacional de Inclusão de Jovens – Projovem Campo no 
			<?=($esfera == 'M' ? 'Município' : 'Estado')?> 
			de <b><?=($esfera == 'M' ? $municipioescola : $estado)?></b> 
			pela Secretaria <?=($municipioaluno ? 'Municipal' : 'Estadual')?> de Educação, 
			sob o nº <b><?=$ninscricao?></b> 
			na Escola <b><?=$nomeescola?></b>, 
			endereço <b><?=$endescola?></b> 
			para início das aulas em <b> ________________________________ .
			
			<br><br>
			
<!-- 			Ressalta-se que: -->
			
<!-- 			<br><br> -->
			
			<div style="padding-left:15px;">
<!-- 				<li>O curso tem duração de 18 meses ininterruptos.</li> -->
<!-- 				<li>Você deverá ter freqüência de 75% nas aulas dadas de cada mês e entregar 75% dos trabalhos pedagógicos do mesmo período, para receber seu auxílio financeiro no valor de R$ 100,00/mês (cem reais), não podendo ultrapassar 18 pagamentos no Programa.</li> -->
<!-- 				<li>Portanto, caso você já tenha participado do Programa, e já tenha recebido algum auxílio financeiro, você receberá apenas os que faltam para completar os 18 auxílios financeiros.</li> -->
<!-- 				<li>Ao final do curso você deverá ter freqüência de 75% nas atividades presenciais e 50% da pontuação distribuída, para ser certificado no Projovem Campo (ensino fundamental/EJA, qualificação profissional e participação cidadã).</li> -->
<!-- 			</div> -->
			
<!-- 			<br><br> -->
			
			Confira abaixo os dados pessoais registrados na matrícula.
			
			<br><br>
			
			<div style="padding-left:15px;">
				
				<b>Número de Matrícula:</b> <?=$ninscricao?>
				<br><b>Turma:</b> <?=$turma?>
				<br><b>CPF:</b> <?=$estcpf?>
				<br><b>Nome:</b> <?=$estnome?>
				<br><b>Data de nascimento:</b> <?=$estdatanascimento?>
				<br><b>Nome da mãe:</b> <?=$estnomemae?>
				<br><b>RG:</b> <?=$rg?>
				<br><b>Endereço:</b> <?=$endaluno?>
				
			</div>
						
			<br><br>

			Declaro que as informações prestadas no ato da matrícula e os dados pessoais informados estão corretos e são verídicos.
			Estou ciente das condições e informações relativas ao recebimento do auxílio financeiro do Projovem Campo e demais informações contidas neste documento.
			
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