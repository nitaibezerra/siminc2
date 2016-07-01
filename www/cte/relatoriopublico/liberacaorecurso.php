<?
$municipio = $_SESSION['muncod'];
$sql = "Select ent.entnumcpfcnpj from entidade.entidade ent
			inner join entidade.funcaoentidade fe on fe.entid = ent.entid
			inner join entidade.endereco ende on ent.entid = ende.entid
		where fe.funid = 1
		and ent.entstatus = 'A'
		and ende.muncod = '".$municipio."'";
$cnpj = $db->pegaUm( $sql );
$ano = date("Y");

require_once APPRAIZ . "includes/Snoopy.class.php";
	$conexao = new Snoopy;
	
	$urlReferencia = "http://www.fnde.gov.br/pls/simad/internet_fnde.liberacoes_result_pc?p_ano=%s&p_uf=%s&p_municipio=%s&p_tp_entidade=&p_cgc=%s";
				
	$url = sprintf($urlReferencia, $ano, $uf, $municipio, $cnpj);
	$conexao->fetch($url);
	$resultado = $conexao->results;
	$resultado = str_replace('#000099','#7E8E47',$resultado);
	$resultado = str_replace('#006699','#acbc73',$resultado);
	$resultado = str_replace('#F8C400','#ccd7a4',$resultado);
	$resultado = str_replace('#FFCC66','#ccd7a4',$resultado);

	$resultado = str_replace('<font face="Tahoma,Arial" size="2" color="#acbc73">','<font face="Tahoma,Arial" color="#333333" size="2">',$resultado);
	$resultado = str_replace('<font face="Tahoma,Arial" size="2" color="#FFFFFF">','<font face="Tahoma,Arial" color="#000000" size="2">',$resultado);
	$resultado = str_replace('Volta a consulta de liberações','',$resultado);
	
	$url2008 = sprintf($urlReferencia, 2008, $uf, $municipio, $cnpj);

	$conexao->fetch($url2008);
	$resultado2008 = $conexao->results;
	$resultado2008 = str_replace('#000099','#7E8E47',$resultado2008);
	$resultado2008 = str_replace('#006699','#acbc73',$resultado2008);
	$resultado2008 = str_replace('#F8C400','#ccd7a4',$resultado2008);
	$resultado2008 = str_replace('#FFCC66','#ccd7a4',$resultado2008);

	$resultado2008 = str_replace('<font face="Tahoma,Arial" size="2" color="#acbc73">','<font face="Tahoma,Arial" color="#333333" size="2">',$resultado2008);
	$resultado2008 = str_replace('<font face="Tahoma,Arial" size="2" color="#FFFFFF">','<font face="Tahoma,Arial" color="#000000" size="2">',$resultado2008);
	$resultado2008 = str_replace('Volta a consulta de liberações','',$resultado2008);

?>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'/>
<table class="tabela" align="center" bgcolor="" cellspacing="1" cellpadding="3" style="font-family:Arial, Verdana; font-size:10px;">
	<tr>
		<td class="tituloPrincipalAbas"><b>LIBERAÇÃO DE RECURSOS (2008)</b></td>
	</tr>
	<tr>
		<td>
			<?=$resultado2008;?>
		</td>
	</tr>
	<tr>
		<td class="tituloPrincipalAbas"><b>LIBERAÇÃO DE RECURSOS (<?php echo $ano ?>)</b></td>
	</tr>

	<tr>
		<td>
			<?=$resultado;?>
		</td>
	</tr>
</table>