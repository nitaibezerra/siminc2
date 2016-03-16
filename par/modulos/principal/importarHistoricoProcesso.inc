<?php 
ini_set("memory_limit","2050M");
set_time_limit(0);

if( $_SESSION['usucpf'] == '' && $_REQUEST['executa'] == 1 ){
	
	global $db;
	
	$sql = "select 
				distinct hwp.* 
			from 
				par.historicowsprocessopar hwp 
			inner join par.empenhopregaoitensenviados e on e.prpid = hwp.prpid and epistatus = 'A'
			where 
				hwp.hwpwebservice = 'enviarManualmenteItensSigarp - Sucesso' 
				AND pular is null
			order by 
				hwp.hwpdataenvio desc
			LIMIT 100";
	
	$dados = $db->carregar($sql);
	
	$conta = 0;
	
	if(is_array($dados)){
		foreach($dados as $historicoProcesso){
			$hwpid = $historicoProcesso['hwpid'];
			$prpid = $historicoProcesso['prpid'];
			$xmlEnvio = simplexml_load_string($historicoProcesso['hwpxmlenvio'])->children();
        	$xmlRetorno = simplexml_load_string($historicoProcesso['hwpxmlretorno'])->children();
        	
        	if( $xmlRetorno->body->params->adesaopregao->nu_sub_acao ){
        		$sbdid = $xmlRetorno->body->params->adesaopregao->nu_sub_acao;
        		$adesao = $xmlRetorno->body->params->adesaopregao->nu_seq_solicitacao_adesao;
	        	
	        	$corretos = $db->pegaLinha("SELECT sbaid, sbdano FROM par.subacaodetalhe WHERE sbdid = ".$sbdid);
	        	$dadosErrados = $db->pegaLinha("SELECT * FROM par.empenhopregaoitensenviados WHERE adesao = ".$adesao);
	        	
//	        	ver( $prpid, $sbdid, $adesao, $corretos, $dadosErrados );
	        	
	        	if( $corretos['sbaid'] <> $dadosErrados['sbaid'] ){
		        	$sqlUpdate .= "UPDATE par.empenhopregaoitensenviados SET sbaid = ".$corretos['sbaid'].", epiano = ".$corretos['sbdano']." WHERE adesao = ".$adesao."; ";
		        	$sqlUpdate .= "UPDATE par.historicowsprocessopar SET pular = 't' WHERE hwpid = ".$hwpid."; ";
		        	$conta++;
	        	} else {
		        	$sqlUpdate .= "UPDATE par.historicowsprocessopar SET pular = 't' WHERE hwpid = ".$hwpid."; ";
		        	$conta++;
	        	}
	        	
	        	if( $sqlUpdate ){
	        		$db->executar($sqlUpdate);
	        		$db->commit();
	        	}
        	}
		}
	}
	
	echo "Foram executados ".$conta." registros.";
	
} elseif( $_SESSION['usucpf'] == '' && $_REQUEST['executa'] == 2 ){
	
	global $db;
	
	$sql = "select 
				distinct hwp.* 
			from 
				par.historicowsprocessopar hwp 
			inner join par.empenhopregaoitensenviados e on e.prpid = hwp.prpid and epistatus = 'A'
			where 
				hwp.hwpwebservice = 'enviarManualmenteItensSigarp - Sucesso' 
				AND pular is null
			
			UNION ALL
			
			select 
				distinct hwp2.* 
			from 
				log_historico.par_historicowsprocessopar hwp2 
			inner join par.empenhopregaoitensenviados e on e.prpid = hwp2.prpid and epistatus = 'A'
			where 
				hwp2.hwpwebservice = 'enviarManualmenteItensSigarp - Sucesso'
				AND pular is null
			LIMIT 50";
	
	$dados = $db->carregar($sql);

	$contaA = 0;
	$contaI = 0;
	
//	ver($dados, d);
//	ver(simec_htmlentities($dados[0]['hwpxmlenvio'], $dados[0]['hwpxmlretorno']), d);
	
	if(is_array($dados)){
		foreach($dados as $historicoProcesso){
			$hwpid = $historicoProcesso['hwpid'];
			$prpid = $historicoProcesso['prpid'];
			$xmlEnvio = simplexml_load_string($historicoProcesso['hwpxmlenvio'])->children();
        	$xmlRetorno = simplexml_load_string($historicoProcesso['hwpxmlretorno'])->children();
        	
        	if( $xmlEnvio->body->params ){
        		foreach( $xmlEnvio->body->params as $envio ){
	        		foreach( $envio->sub_acao as $subs ){
        				$subacao = (integer)$subs->nu_sub_acao;
						foreach($subs->item as $itensEnvio){
							$itemEnvio[$subacao][(integer)$itensEnvio->nu_seq_item]['pregao'] = (integer)$itensEnvio->nu_seq_pregao;						
							$itemEnvio[$subacao][(integer)$itensEnvio->nu_seq_item]['quantidade'] = (integer)$itensEnvio->qt_item;						
							$itemEnvio[$subacao][(integer)$itensEnvio->nu_seq_item]['valor'] = $itensEnvio->vl_pagamento;		
						}
	        		}
        		}
        	}

        	if( $xmlRetorno->body->params->adesaopregao->nu_sub_acao ){
				foreach( $xmlRetorno->body->params as $subacoes ){
					foreach($subacoes->adesaopregao as $dadoPregao){
						$subacaoRetorno = (integer)$dadoPregao->nu_sub_acao;
		        		$adesao = (integer)$dadoPregao->nu_seq_solicitacao_adesao;
						foreach($dadoPregao->item->nu_seq_item as $itens){
							$itemEnvio[$subacaoRetorno][(integer)$itens]['adesao'] = $adesao;						
//							$item[$adesao][] = (integer)$itens;
//							ver($itemEnvio);
						}
					}
				}
				
				if(is_array($itemEnvio)){
					foreach($itemEnvio as $itensParaAtualizar){
						foreach($itensParaAtualizar as $itemAtualizar => $dadoAtualizar){
							$testaItem = $db->pegaUm("SELECT epiid FROM par.empenhopregaoitensenviados WHERE idsigarp = ".$itemAtualizar." AND adesao = ".$dadoAtualizar['adesao']);
							if( $testaItem ){ // O item está na tabela
								if(strpos($dadoAtualizar['valor'], ',')){
									$vl = str_replace(array('.', ','), array('', '.'), $dadoAtualizar['valor']);
								} else {
									$vl = $dadoAtualizar['valor'];
								}
								$sqlAtualizar .= "UPDATE par.empenhopregaoitensenviados SET epiquantidade = ".$dadoAtualizar['quantidade'].", epivalor = ".$vl.", epipregao = ".$dadoAtualizar['pregao']." WHERE adesao = ".$dadoAtualizar['adesao']." AND idsigarp = ".$itemAtualizar."; ";
								$contaA++;
							} else { // O item não está na tabela
								if(strpos($dadoAtualizar['valor'], ',')){
									$vl = str_replace(array('.', ','), array('', '.'), $dadoAtualizar['valor']);
								} else {
									$vl = $dadoAtualizar['valor'];
								}
								$sqlInsert = "INSERT INTO par.empenhopregaoitensenviados ( prpid, sbaid, idsigarp, adesao, usucpf, epistatus, epiano, epiquantidade, epivalor, epipregao ) 
										SELECT prpid, sbaid, ".$itemAtualizar.", adesao, usucpf, epistatus, epiano, ".$dadoAtualizar['quantidade'].", ".$vl.", ".$dadoAtualizar['pregao']." FROM par.empenhopregaoitensenviados WHERE adesao = ".$dadoAtualizar['adesao']." LIMIT 1; ";								
				        		$db->executar($sqlInsert);
								$contaI++;
							}
							$sqlAtualizar .= "UPDATE par.historicowsprocessopar SET pular = 't' WHERE hwpid = ".$hwpid."; UPDATE log_historico.par_historicowsprocessopar SET pular = 't' WHERE hwpid = ".$hwpid."; ";
						}
					}
				}
				
	        	if( $sqlAtualizar ){
	        		$db->executar($sqlAtualizar);
	        		$db->commit();
	        	}
        	}
		}
	}
	
	echo "Foram executados ".$contaA." alterações em registros e ".$contaI." inclusões.";
	
} else {
$_SESSION['par']['arEstuf'] = pegaResponssabilidade('1');
if (!$_SESSION['par']['boPerfilSuperUsuario'] && !$_SESSION['par']['boPerfilConsulta']) {
//	if( (!isset($_SESSION['par']['arEstuf']) && !count($_SESSION['par']['arEstuf'])) && $_SESSION['par']['estuf'] && !(in_array(PAR_PERFIL_ANALISTA_MERITOS, $perfis)) ){
    if ((!isset($_SESSION['par']['arEstuf']) && !count($_SESSION['par']['arEstuf'])) && $_SESSION['par']['estuf']) {
        header("Location: par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore");
        exit;
    }
}

include APPRAIZ . "includes/cabecalho.inc";
echo'<br>';

//echo montarAbasArray(criaAbaPar(), "par.php?modulo=principal/listaEstados&acao=A");
$titulo_modulo = "Carga de Adesões para o SIGARP";
monta_titulo($titulo_modulo, '');

# Estado
$_SESSION['par']['itrid'] = 1;

/**
 * Verifica se o usuario logado e super usuario
 * 
 * @name isSuperUsuario()
 * @author Ruy Ferreira <ruy.silva@mec.gov.br>
 * @since 18/03/2013
 * @return boolean
 */
function isSuperUsuario() {
    global $db;
    if ($db->testa_superuser()) {
        return true;
    }
    $usuario = $_SESSION['usucpf'];
    $sql = sprintf(
            "select count( * )
		from seguranca.perfilusuario
		where
			usucpf = '%s' and
			pflcod = %d", $usuario, PERFIL_SUPERUSUARIO
    );
    return (boolean) $db->pegaUm($sql);
}


$intQtdHistoricoRestante = 0;
$intQtdHistoricoPulado = 0;

// Se for super usuario e formulario for submetido.
if (isSuperUsuario() && $_POST['boo_importar'] == 2){
    $controle = new ProcessoItemAdesaoPregaoControle();
    
    // Log de importacao dos itens de cada historico com sucesso e com erro.
    $result = $controle->transferirVariosHistoricoProcesso($_POST['qtd_importar']);
    
    // Quantidade de historico restantes
    $intQtdHistoricoRestante = $controle->carregarHistoricoProcessoQtdRestante();
    
    // Quantidade de historico que foram pulados
    $intQtdHistoricoPulado = $controle->carregarHistoricoProcessoQtdPulado();
}

?>
<?php if (isSuperUsuario()):?>
    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="90%">
        <tbody>
            <tr align="center">
                <td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4" width="90%">
                    <form method="post" name="formulario-importar" id="formulario-importar"  width="90%">
                        <input type="hidden" name="boo_importar" value="2"/>
                        <div  width="90%">
                            Quantidade à ser importado: <input type="text" name="qtd_importar" value="<?php echo ($_POST['qtd_importar'])? $_POST['qtd_importar'] :'500' ?>" />
                        </div>
                        <div>
                            <input type="submit" name="pesquisar" value="Importar histórico documento" />
                        </div>
                        <div>
                            Com sucesso: <b><?php echo count($result['sucesso']) ?></b>
                        </div>
                        <div>
                            Com erro: <b><?php echo count($result['erro']) ?></b>
                        </div>
                        <div>
                            Faltam: <b><?php echo $intQtdHistoricoRestante ?></b>
                        </div>
                        <div>
                            Total pulado: <b><?php echo $intQtdHistoricoPulado ?></b>
                        </div>
                    </form>
                </td>
            </tr>
        </tbody>
    </table>
    <table class="listagem" width="95%" cellspacing="0" cellpadding="2" border="0" align="center">
        <thead>
            <tr>
                <td width="20%" class="title" valign="top" align="95%" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" bgcolor="">
                    <b>ID HISTORICO</b>
                </td>
                <td><b>ITENS</b></td>
            </tr>
        </thead>
        <tbody>
            <?php if($result): ?>
            <?php $color = "" ?>
                <?php foreach($result['sucesso'] as $value): ?>
                <tr bgcolor="<?php echo $color ?>" onmouseout="this.bgColor='<?php echo $color ?>';" onmouseover="this.bgColor='#ffffcc';">
                    <td>
                        <?php echo $value['ID_HISTORICO'] ?>
                    </td>
                    <td>
                        <?php foreach($value['ID_ITENS'] as $item): ?>
                            | <?php echo $item ?>
                        <?php endforeach ?>
                    </td>
                </tr>
                <?php $color = ($color)? '' : '#F7F7F7'; ?>
                <?php endforeach; ?>
            <?php endif ?>
        </tbody>
    </table>
    <table class="listagem" width="95%" cellspacing="0" cellpadding="2" border="0" align="center">
        <thead>
            <tr>
                <td rowspan="2">
                    <b style="color: red">ID HISTORICO</b>
                </td>
                <td colspan="7" style="text-align: center;">
                    <b style="color: red">ITENS</b>
                </td>
            </tr>
            <tr>
                <td>prpid</td>
                <td>icoid</td>
                <td>pianumeroadesao</td>
                <td>piaquantidadeitens</td>
                <td>piavaloritens</td>
                <td>piastatus</td>
            </tr>
        </thead>
        <tbody>
            <?php if($result): ?>
                <?php $color = "" ?>
                <?php foreach($result['erro'] as $value): ?>
                <tr bgcolor="<?php echo $color ?>" onmouseout="this.bgColor='<?php echo $color ?>';" onmouseover="this.bgColor='#ffffcc';">
                    <td rowspan="<?php echo count($value['ITENS']) + 1 ?>">
                        <?php echo $value['ID_HISTORICO'] ?>
                    </td>
                </tr>
                    <?php foreach($value['ITENS'] as $valueItem): ?>
                    <tr bgcolor="<?php echo $color ?>" onmouseout="this.bgColor='<?php echo $color ?>';" onmouseover="this.bgColor='#ffffcc';">
                        <td><?php echo $valueItem->prpid ?></td>
                        <td><?php echo $valueItem->icoid ?></td>
                        <td><?php echo $valueItem->pianumeroadesao ?></td>
                        <td><?php echo $valueItem->piaquantidadeitens ?></td>
                        <td><?php echo $valueItem->piavaloritens ?></td>
                        <td><?php echo $valueItem->piastatus ?></td>
                    </tr>
                    <?php endforeach; ?>
                <?php $color = ($color)? '' : '#F7F7F7'; ?>
                <?php endforeach; ?>
            <?php endif ?>
        </tbody>
    </table>
<?php endif ?>
<div id="divDebug"></div>
<?php } ?>