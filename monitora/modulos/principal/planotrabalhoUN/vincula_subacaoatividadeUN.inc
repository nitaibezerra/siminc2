<?php
require_once APPRAIZ . "monitora/www/planotrabalhoUN/_funcoes.php";
require_once APPRAIZ . "monitora/www/planotrabalhoUN/_constantes.php";
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<?

if($_POST['gravar']) {
	
	$db->executar("DELETE FROM monitora.pi_planointernoatividade WHERE atiid='".$_POST['atiid']."'");	
	
	if($_POST['pliid']) {
		foreach($_POST['pliid'] as $pliid) {
			
			$sql = "INSERT INTO monitora.pi_planointernoatividade(pliid, atiid)
    				VALUES ('".$pliid."', '".$_POST['atiid']."');";
			
			$db->executar($sql);
			
		}
	}
	
	$db->commit();
	
	die("<script>alert('Gravado com sucesso');window.opener.location.replace(window.opener.location);window.close();</script>");
}

echo "<br />";


$unidade = "";
if(!$db->testa_superuser()){
	$unidade = carrega_unidade_titulo();	
}

monta_titulo("Atividade/PI",$unidade);

$sbaid = $db->pegaUm("SELECT sbaid FROM monitora.pi_subacaoatividade WHERE atiid='".$_REQUEST['atiid']."'");

if(!$sbaid) die("<script>alert('Não existe subação para a atividade');window.close();</script>");

?>
<form method="post"  name="formulario" action="monitora.php?modulo=principal/planotrabalhoUN/vincula_subacaoatividadeUN&acao=A">

<input type="hidden" name="atiid" value="<? echo $_REQUEST['atiid']; ?>">

<table width="100%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" align="right">Atividade:</td>
	<td><? echo $db->pegaUm("SELECT atidescricao FROM pde.atividade WHERE atiid='".$_REQUEST['atiid']."'"); ?></td>
</tr>	
<tr>
	<td class="SubTituloDireita" align="right">Subação:</td>
	<td><? echo $db->pegaUm("SELECT sbatitulo FROM monitora.pi_subacao WHERE sbaid='".$sbaid."'"); ?></td>
</tr>	
<tr>
	<td colspan="2">
	<?
	$sql = "SELECT '<center><input type=checkbox name=pliid[] value='|| p.pliid ||' '|| CASE WHEN (SELECT atiid FROM monitora.pi_planointernoatividade WHERE pliid=p.pliid ) IS NULL THEN '' ELSE 'checked' END || '></center>' as acao,
				 
				p.plicod as plicod,
				
				coalesce(p.plititulo,'Não preenchido') as titulo,
				
				coalesce(SUM(pt.pipvalor),0) as total,
				
				CASE WHEN p.plisituacao = 'P' THEN ' <font color=\"red\">Pendente</font> '
					 WHEN p.plisituacao = 'A' THEN ' <font color=\"green\">Aprovado</font> '
					 WHEN p.plisituacao = 'H' THEN ' <font color=\"blue\">Homologado</font> '
					 WHEN p.plisituacao = 'R' THEN	' <font color=\"#3F85FF\">Revisado</font> '
					 WHEN p.plisituacao = 'C' THEN	' <font color=\"#AF7817\">Cadastrado no SIAFI</font> '
					 WHEN p.plisituacao = 'E' THEN	' <font color=\"#EAC117\">Enviado para Revisão</font> ' END as situacao,
					 
				(SELECT usunome ||' por '||to_char(pihdata, 'dd/mm/YYYY hh24:mi') FROM monitora.pi_planointernohistorico p1 LEFT JOIN seguranca.usuario u1 ON u1.usucpf = p1.usucpf WHERE p1.pliid=p.pliid ORDER BY p1.pihdata DESC LIMIT 1) as hst
				
				FROM monitora.pi_planointerno p 
				LEFT JOIN monitora.pi_planointernoptres pt ON pt.pliid = p.pliid 
				LEFT JOIN monitora.pi_planointernoatividade pla ON pla.pliid = p.pliid
				WHERE 
				 p.sbaid = ".$sbaid." AND
				p.plistatus = 'A' AND (pla.atiid IS NULL OR pla.atiid = '".$_REQUEST['atiid']."')
				AND p.pliano = '{$_SESSION['exercicio']}'
				AND (p.plisituacao = 'A' OR p.plisituacao = 'C')
				GROUP BY p.pliid, p.plicod,p.plititulo,p.plidata,p.plisituacao 
				ORDER BY p.plidata DESC";

	$cabecalho = array("","Código do PI","Título","Valor Previsto(R$)","Situação","Última atualização");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2); 
	?>
	</td>
</tr>
<tr>
<td class="SubTituloDireita" colspan="2"><input type="submit" name="gravar" value="Salvar"></td>
</tr>	

</table>
</form>