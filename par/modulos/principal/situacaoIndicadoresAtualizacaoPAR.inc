<?php
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

if( !$_SESSION['par']['inuid'] ){
	echo '<script>
				alert("Faltam dados da sessão!");
				window.location="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore";
			</script>';
	die;
}

$mnuid = verificaAbasAtualizacao( $_SESSION['par']['inuid'] );

$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];

//$db->cria_aba( $abacod_tela, $url, '' );
echo cabecalho();
echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td style="color: blue; font-size: 22px">
			<a href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore">';
				echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['par']['itrid']);
			echo '</a>';

$desc = "";
if( situacaoIndicadoresAtualizacaoPAR($_SESSION['par']['inuid']) == false ){
	$desc = "<font color='red'>Você precisa regularizar as pendências indicadas abaixo para poder prosseguir com a atualização do PAR</font>";
}

$db->cria_aba( $abacod_tela, $url, $parametros, $mnuid );
monta_titulo( 'Atualização do PAR', $desc );

$dados = $db->pegaLinha("SELECT * FROM par.atualizacaopar WHERE inuid = ".$_SESSION['par']['inuid']);

$habil = $dados['atpjustificativaideb'] ? 'N' : 'S';
$habilitaBotao = $dados['atpjustificativaideb'] ? 'disabled="disabled"' : '';

$entidade = $db->pegaLinha("SELECT itrid, CASE WHEN itrid = 1 THEN estuf ELSE muncod END as entidade, CASE WHEN itrid = 1 THEN 'estuf' ELSE 'muncod' END as tipoentidade FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par']['inuid']);

$dadosTermo = $db->pegaUm("SELECT DISTINCT
									COALESCE(count(dop.dopid),0) as conta
								FROM 
									par.vm_documentopar_ativos dop
								INNER JOIN par.processopar prp ON prp.prpid = dop.prpid and prp.prpstatus = 'A'
								INNER JOIN par.instrumentounidade iu ON iu.inuid = prp.inuid
								where 
									dop.mdoid = 20 AND 
									iu.".$entidade['tipoentidade']." = '".$entidade['entidade']."' AND
									dop.dopid NOT IN ( SELECT dopid FROM par.documentoparvalidacao dpv )");

$dadosHabilita = $db->pegaUm("SELECT iuesituacaohabilita FROM par.instrumentounidadeentidade WHERE iuedefault = 't' AND inuid = ".$_SESSION['par']['inuid']);
	
$esfera = $entidade['tipoentidade'] == 'estuf' ? 'E' : 'M'; 

if( $esfera == 'E' || $_SESSION['par']['estuf'] == 'DF' ){
	$sql = "SELECT estado,stoid, COUNT(1) AS qtdbloqueio FROM (
				SELECT
					o.obrid,
					o.estuf AS estado,
					o.situacaoobra AS stoid,
					CASE WHEN o.situacaoobra IN (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") AND desterminodeferido IS NOT NULL AND desterminodeferido >= now() THEN 0
						WHEN o.situacaoobra IN (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") AND coalesce(o.diasprimeiropagamento, o.diasinclusao) > 540 THEN 1 
						ELSE 0 end as bloqueiolicitacao,
					CASE WHEN o.situacaoobra IN (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.") and o.diasultimaalteracao > 60 THEN 1 ELSE 0 END as bloqueioexecucaoparalisada
						
				FROM
					obras2.vm_obras_situacao_estadual o
				WHERE
					o.inuid = {$_SESSION['par']['inuid']}
					AND o.situacaoobra IN (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.", ".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.")
			) t WHERE bloqueiolicitacao = 1 or bloqueioexecucaoparalisada = 1
			GROUP BY estado, stoid";
	
	$dadosObra = $db->pegaLinha($sql);
} else {
	$sql = "SELECT estado,stoid, COUNT(1) AS qtdbloqueio FROM (
				SELECT
					o.obrid,
					o.estuf AS estado,
					o.situacaoobra AS stoid,
					CASE WHEN o.situacaoobra IN (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") AND desterminodeferido IS NOT NULL and desterminodeferido >= now() THEN 0
						WHEN o.situacaoobra IN (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") AND coalesce(o.diasprimeiropagamento, o.diasinclusao) > 540 THEN 1 
						ELSE 0 END AS bloqueiolicitacao,
					CASE WHEN o.situacaoobra IN (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.") AND o.diasultimaalteracao > 60 THEN 1 ELSE 0 END AS bloqueioexecucaoparalisada
						
				FROM
					obras2.vm_obras_situacao_municipal o
				WHERE
					o.inuid = {$_SESSION['par']['inuid']}
					AND o.situacaoobra IN (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.", ".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.")
			) t WHERE bloqueiolicitacao = 1 OR bloqueioexecucaoparalisada = 1
			GROUP BY estado, stoid";
	$dadosObra = $db->carregar($sql);
}
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	function salvarIDEB(){
		if($('#justificativa').val() == ''){
			alert('Justifique seu índice, apontando ações para melhoria.');
			return false;
		}
		$('#requisicao').val('salvar');
		$('#formulario').submit();
	}
</script>
<form name="formulario" id="formulario" action="" method="post">
	<input type="hidden" id="requisicao" name="requisicao">	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<colgroup>
			<col width="25%" />
			<col width="75%" />
	</colgroup>
	<tr>
		<td class="SubTituloDireita" valign="top">Preenchimento do monitoramento de Obras atualizado: </td>
		<td>
			<?php
			if( !($esfera == 'E' || $_SESSION['par']['estuf'] == 'DF') ){
				if( sizeof($dadosObra) > 0 && is_array($dadosObra) ){
					foreach( $dadosObra as $obra ) {
						if ($obra['diaslicitacao'])
						{
							echo '<font color="red">Obra ID ' . $obra['obrid'] . ' com licitação não concluída após ' . $obra['diaslicitacao'] . ' dia(s) - ' . $obra['obrdesc'] . '</font><br/>';
						}
						elseif ($obra['diasatualizacao'])
						{
							echo '<font color="red">Obra ID ' . $obra['obrid'] . ' com ' . $obra['diasatualizacao'] . ' dia(s) sem atualização - ' . $obra['obrdesc'] . '</font><br/>';
						}
					}
				} else {
					echo '<font color="green">Sem pendências no Monitoramento de obras 1.</font><br/>';
				}
			}
			
            $aPendencias = getObrasPendentesPAR($_SESSION['par']['muncod'], $_SESSION['par']['estuf']);
            if($aPendencias && is_array($aPendencias)){
                echo '<ul>';
                foreach ($aPendencias as $pendencia) {
                    echo '<li style="color: red;">(' . $pendencia['obrid'] . ') - ' . $pendencia['obrnome'] . ' Motivo: ' . $pendencia['pendencia'] . '. Exec. ' . number_format($pendencia['obrpercentultvistoria'], 2, ',', '.') . '%</li>';
                }
                echo '</ul>';
            }

			/* ORdenação por OBRID
			 * Feito em 05/12/2013 a pedido de Daniel Areas feito por Eduardo Dunice
			 * */
// 			if( $esfera == 'E' || $_SESSION['par']['estuf'] == 'DF' ){
// 				if( $dadosObra['qtdbloqueio'] > 0 ){
// 					$sql = "SELECT * FROM (
// 									SELECT
// 										o.*,
// 										o.estuf AS estado,
// 										o.situacaoobra as stoid,
// 										case when o.situacaoobra in (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") and desterminodeferido is not null and desterminodeferido >= now() then 0
// 											when o.situacaoobra in (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") and coalesce(o.diasprimeiropagamento, o.diasinclusao) > 540 then 1 
// 											else 0 end as bloqueiolicitacao,
// 										case when o.situacaoobra in (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.") and o.diasultimaalteracao > 60 then 1 else 0 end as bloqueioexecucaoparalisada
// 										--count(o.obrid) as qtdobras
											
// 									FROM
// 										obras2.v_obras_situacao_estadual o
// 									WHERE
// 										o.inuid = {$_SESSION['par']['inuid']}
// 										--o.estuf = 'BA'
// 										AND o.situacaoobra in (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.", ".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.")
// 								) t where bloqueiolicitacao = 1 or bloqueioexecucaoparalisada = 1
// 								order by obrid, bloqueiolicitacao desc, bloqueioexecucaoparalisada desc
// 					";
// 					$obras = $db->carregar($sql);
// 					foreach( $obras as $obra ) {
// 						if ($obra['bloqueiolicitacao'])
// 						{
// 							echo '<font color="red">Obra ID ' . $obra['obrid'] . ' com licitação não concluída após ' . $obra['diasinclusao'] . ' dia(s) - ' . $obra['obrnome'] . '</font><br/>';
// 						}
// 						elseif ($obra['bloqueioexecucaoparalisada'])
// 						{
// 							echo '<font color="red">Obra ID ' . $obra['obrid'] . ' com ' . $obra['diasultimaalteracao'] . ' dia(s) sem atualização - ' . $obra['obrnome'] . '</font><br/>';
// 						}
// 					}
// 				} else {
// 					echo '<font color="green">Sem pendências.</font>';
// 				}
// 			} else {
				
// 				if( sizeof($dadosObra) > 0 && is_array($dadosObra) ){
// 					foreach( $dadosObra as $obra ) {
// 						if ($obra['diaslicitacao'])
// 						{
// 							echo '<font color="red">Obra ID ' . $obra['obrid'] . ' com licitação não concluída após ' . $obra['diaslicitacao'] . ' dia(s) - ' . $obra['obrdesc'] . '</font><br/>';
// 						}
// 						elseif ($obra['diasatualizacao'])
// 						{
// 							echo '<font color="red">Obra ID ' . $obra['obrid'] . ' com ' . $obra['diasatualizacao'] . ' dia(s) sem atualização - ' . $obra['obrdesc'] . '</font><br/>';
// 						}
// 					}
// 				} else {
// 					echo '<font color="green">Sem pendências no Monitoramento de obras 1.</font><br/>';
// 				}
				

				

// 				$sql = "SELECT * FROM (
// 							SELECT
// 								o.*,
// 								o.estuf AS estado,
// 								o.situacaoobra as stoid,
// 								CASE WHEN o.situacaoobra IN (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") AND desterminodeferido IS NOT NULL AND desterminodeferido >= now() THEN 0
// 									WHEN o.situacaoobra IN (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") AND coalesce(o.diasprimeiropagamento, o.diasinclusao) > 540 THEN 1 
// 									ELSE 0 END as bloqueiolicitacao,
// 								CASE WHEN o.situacaoobra IN (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.") AND o.diasultimaalteracao > 60 THEN 1 ELSE 0 END AS bloqueioexecucaoparalisada
									
// 							FROM
// 								obras2.v_obras_situacao_municipal o
// 							WHERE
// 								o.muncod = '{$entidade['entidade']}'
// 								AND o.situacaoobra in (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.", ".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.")
// 						) t WHERE bloqueiolicitacao = 1 OR bloqueioexecucaoparalisada = 1
// 						ORDER BY obrid, bloqueiolicitacao DESC, bloqueioexecucaoparalisada DESC";
			
// 				$dadosObra = $db->carregar($sql);
// 				if( sizeof($dadosObra) > 0 && is_array($dadosObra) ){
// 					foreach( $dadosObra as $obra ) {
// 						if ($obra['bloqueiolicitacao'])
// 						{
// 							echo '<font color="red">Obra ID ' . $obra['obrid'] . ' com licitação não concluída após ' . $obra['diasinclusao'] . ' dia(s) - ' . $obra['obrnome'] . '</font><br/>';
// 						}
// 						elseif ($obra['bloqueioexecucaoparalisada'])
// 						{
// 							echo '<font color="red">Obra ID ' . $obra['obrid'] . ' com ' . $obra['diasultimaalteracao'] . ' dia(s) sem atualização - ' . $obra['obrnome'] . '</font><br/>';
// 						}
// 					}
// 				} else {
// 					echo '<font color="green">Sem pendências no Monitoramento de obras 2.0.</font>';
// 				}
// 			}
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Situação Habilita: </td>
		<td>
			<?=$dadosHabilita; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Existência de Termo de Compromisso sem validação: </td>
		<td>
			<?=$dadosTermo > 0 ? '<font color="red">Existem ' . $dadosTermo . ' Termo(s) sem validação.</font>' : '<font color="green">Sem pendências.</font>'; ?>
		</td>
	</tr>
	</table>
</form>
		</td>
	</tr>
</table>