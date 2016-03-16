<?php
$_SESSION['rehuf_var']['entid'] = $_GET['entid'] ? $_GET['entid'] : $_SESSION['rehuf_var']['entid'];
if(!$_SESSION['rehuf_var']['esuid']){
	$dadosestruturaunidadeid = $db->pegaUm("SELECT esuid FROM rehuf.estruturaunidade WHERE entid = '".$_SESSION['rehuf_var']['entid']."'");
}
$_SESSION['rehuf_var']['esuid'] = !$_SESSION['rehuf_var']['esuid'] ? $dadosestruturaunidadeid : $_SESSION['rehuf_var']['esuid'];

if(validaVariaveisSistema()) {
	echo "<script>
			alert('Problemas nas variáveis do sistema.');
			window.location='?modulo=inicio&acao=A';
		  </script>";
	exit;
}

/* Tratamento de segurança */
$permissoes = verificaPerfilRehuf();
validaAcessoHospital($permissoes['verhospitais'], $_SESSION['rehuf_var']['entid']);
/* FIM - Tratamento de segurança */

if($_REQUEST['exec_function']){
	if (function_exists($_REQUEST['exec_function'])){
		$_REQUEST['exec_function']($_REQUEST);
	}	
	exit;
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

// pregão sem abas
$_SESSION['rehuf_var']['pregaosemabas'] = false;
/* montando o menu */
echo montarAbasArray(carregardadosmenurehuf(), $_SERVER['REQUEST_URI']);

$titulo_modulo = "Lista de Pregões Homologados";
monta_titulo( $titulo_modulo, '&nbsp;' );
monta_cabecalho_rehuf($_SESSION['rehuf_var']['entid']);

/*
 $sql = "select
			*
		from
			carga.pregao_homologado_5";
$arrDados = $db->carregar($sql);

foreach($arrDados as $d)
{
	$data_inicio = trim(explode(" a ",$d['itedatainiciovigencia'])[0]);
	$data_inicio = formata_data_sql($data_inicio);
	$data_fim = trim(explode(" a ",$d['itedatainiciovigencia'])[1]);
	$data_fim = formata_data_sql($data_fim);
	$data_fim = !$data_fim ? "date '$data_inicio' + interval '1 year'" : "'".$data_fim."'";
	
	$d['preid'] = 5;
	
	$sqlI.= "insert into rehuf.itemhomologado (preid,itecatmat,itedescricao,iteapresentacao,itevalorhomologado,iteqtdeanual,itefabricante,iteataregistro,itedatainiciovigencia,itedatafimvigencia,itestatus) values
	({$d['preid']},{$d['itecatmat']},'{$d['itedescricao']}','{$d['iteapresentacao']}',".str_replace(array(".",","),array("","."),$d['itevalorhomologado']).",".str_replace(array(".",","),array("","."),$d['iteqtdeanual']).",
			'{$d['itefabricante']}','{$d['iteataregistro']}','$data_inicio',$data_fim,'A'
	);
	<br />";
}
dbg($sqlI);
*/

//Regra - Para contabilizasr precisa preencher o valor, tipo de aquisição e CNPJ; ou preecher apenas observação.

$sql = "select
			'<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer\" onclick=\"window.location=\'rehuf.php?modulo=pregao/itemPregaoHomologado&acao=A&preid=' || pre.preid || '\'\" /> ',
			preobjeto,
			count(ite.iteid) as itens,
			count(hit.hitid) as itens_preenchidos,
			case when count(hit.hitid) > 0
				then count(hit.hitid)/count(ite.iteid)::numeric(12,2)*100
			else 0
			end as porcentagem
		from
			rehuf.pregaohomologado pre
		left join
			rehuf.itemhomologado ite ON ite.preid = pre.preid and ite.itestatus = 'A'
		left join
			rehuf.hospitalitemhomologado hit on hit.iteid = ite.iteid and
		 ( (hitvalorunitario is not null and hitcnpjfornecedor is not null and tpaid is not null and entid = {$_SESSION['rehuf_var']['entid']}) or (hitobs is not null and entid = {$_SESSION['rehuf_var']['entid']}) )
		where
			prestatus = 'A'
		group by
			pre.preid,pre.preobjeto
		order by
			pre.preid,pre.preobjeto asc";
//dbg($sql);
$cabecalho = array("Preencher", "Código - Objeto do Pregão","Itens Homologados","Itens Preenchidos","Preenchimento (%)");
$db->monta_lista($sql,$cabecalho,50,5,'N','center','');

?>