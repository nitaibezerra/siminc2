<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'/>

<?php
$municipio = $_SESSION['muncod'];
//$estado = $_SESSION['estado'];
$sql = "Select ent.entnumcpfcnpj from entidade.entidade ent
inner join entidade.endereco ende on ent.entid = ende.entid
inner join entidade.funcaoentidade efe on efe.entid = ent.entid
where efe.funid = 1
and ent.entstatus = 'A'
and ende.muncod = '".$municipio."'";
$cnpj = $db->pegaUm( $sql );
$ano = date("Y");

?>

 
<?PHP  ?>

<table class="tabela" align="center" bgcolor="" cellspacing="1" cellpadding="3" style="font-family:Arial, Verdana; font-size:10px;">
	<tr>
	<td class="tituloPrincipalAbas"><b>LIBERAÇÃO DE RECURSOS</b></td>
	</tr>
	<tr>
		<td>
<iframe marginheight="0" marginwidth="0" name="Localtermo" 
src="http://www.fnde.gov.br/pls/simad/internet_fnde.liberacoes_result_pc?p_ano=<?=$ano;?>&p_uf=<?=$uf;?>&p_municipio=<?=$municipio;?>&p_tp_entidade=&p_cgc=<?=$cnpj; ?>" 
width="100%" frameborder="0" allowtransparency="true" height="900pt" scrolling="auto"></iframe>
		</td>
	</tr>
</table>
