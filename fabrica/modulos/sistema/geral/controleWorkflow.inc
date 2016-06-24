<?php
include APPRAIZ."fabrica/classes/ControleWorkFlow.class.inc";

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// carregando o menu
$menu[0] = array("descricao" => "Listar Controles do WorkFlow", "link"=> "fabrica.php?modulo=sistema/geral/listarControleWorkflow&acao=A");
$menu[1] = array("descricao" => "Cadastrar Controles do WorkFlow", "link"=> "fabrica.php?modulo=sistema/geral/cadControleWorkflow&acao=A");
$menu[2] = array("descricao" => "Executar Script de Controles do WorkFlow", "link"=> "fabrica.php?modulo=sistema/geral/controleWorkflow&acao=A");
echo montarAbasArray($menu, "fabrica.php?modulo=sistema/geral/controleWorkflow&acao=A");

$titulo = "Script de Controle de WorkFlow";
monta_titulo( $titulo, '&nbsp;' );

$controleWorkFlow = new ControleWorkFlow();
//$arrWhere = array("ctwdtexecucao is not null");
$arrDados = $controleWorkFlow->recuperarTodos("*",$arrWhere); ?>

<form name="form_listar_controle_workflow" id="form_listar_controle_workflow" method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td>

<?php
if(is_array($arrDados)):
	echo "<b>Aguarde... Atualizando documentos.</b><br /><br />";
	foreach($arrDados as $dado):
		$sql = "select
					distinct doc.docid
				from
					workflow.documento doc
				inner join
					workflow.historicodocumento his ON his.docid = doc.docid
				where
					esdid = {$dado['esdid']}
				and
					doc.docid not in ( select distinct docid from fabrica.controleworkflowdocumento cwd where cwd.ctwid = ".$dado['ctwid'].") ";
		
		$dado['ctwtemporesp'] = (!$dado['ctwtemporesp'] || $dado['ctwtemporesp'] == "" ? 0 : $dado['ctwtemporesp']);
		
		if($dado['ctwtempoexec'] && $dado['ctwtempoexec'] != "0"):
			$sql .= " and 
						(htddata + interval '".($dado['ctwtemporesp'] - $dado['ctwtempoexec'])." hour') < now()";
		else:
			$sql .= " and 
						(htddata + interval '".$dado['ctwtemporesp']." hour') < now()";
		endif;
		$arrDocid = false;
		$arrDocid = $db->carregar($sql);
		if(is_array($arrDocid)):
			$sqlInsert = "";
			foreach($arrDocid as $docid):
				$sqlInsert .= "insert into fabrica.controleworkflowdocumento (ctwid,docid,dtinsercao) values ({$dado['ctwid']},{$docid['docid']},now()); ";
			endforeach;
			$sqlInsert .= "update fabrica.controleworkflow set ctwdtexecucao = now() where ctwid = {$dado['ctwid']}; ";
			$db->executar($sqlInsert);
			
			if($dado['ctwfuncao'] && $dado['ctwfuncao'] != "" && $dado['ctwtemporesp']){
				//$funcao = substr($dado['ctwfuncao'], 0, strpos($dado['ctwfuncao'],"("));
				$funcao = "mandaEmailOSFinalizada";
				if (function_exists($funcao)) {
					$erro = "";
					$corpoEmail = "Teste de email.<br /><b>Teste!</b>";
					$arrEmailDestino = array($_SESSION['email_sistema']);
					call_user_func_array($funcao,array($corpoEmail,$arrEmailDestino));
					$erro = "(Função '$funcao' executada com sucesso!)";
				}else{
					$erro = "(Não foi possível executar a função '$funcao'! Favor verificar se ela esta presente no arquivo '_funcoes.php')";
				}
			}
		endif;
		echo "* estid ".$dado['esdid']." ".(is_array($arrDocid) ? count($arrDocid) : 0)." documento(s) atualizado(s) $erro;<br />";
	endforeach;
	echo "<br /><b>Documento(s) Atualizando(s) com Sucesso!</b><br />";
endif; ?>

</td>
	</tr>
		</table>