<?php

if($_REQUEST['requisicao'] == 'downloadArquivo'){
    $arqid = $_GET['arqid'];
    if ($arqid) {
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $file = new FilesSimec(null, null, "obras2");
        $file->getDownloadArquivo($arqid);
    }

    die('<script>
                    window.history.go(-1);
             </script>');
}

// empreendimento || obra || orgao
verificaSessao( 'empreendimento' );

$_SESSION['obras2']['semMarcadagua'] = true;

$empid          = $_SESSION['obras2']['empid'];
$empreendimento = new Empreendimento( $empid );
$obra           = new Obras();
$obra->popularDadosObjeto($obra->pegaObraPorEmpid($empid));

$sosid = ($_REQUEST['sosid'] ? $_REQUEST['sosid'] : $_SESSION['obras2']['sosid']);

$dadosMi = pegaDadosTecnologiaMi ($obra->obrid, $_SESSION['obras2']['sueid'], $sosid);
$qstescopo = ($dadosMi) ? $dadosMi['qstescopo'] : "SE";

if(!empty($_REQUEST['iqop'])){
    conteudoDivQuestionarioObraParalisada('impressao');
    die();
}

function testaSelecionado( $arqid, $sueid ){

	global $db;

	$sql = "SELECT DISTINCT
				CASE WHEN aqs.arqid IS NOT NULL OR ass.arqid IS NOT NULL THEN 'f_selected' ELSE '' END
			FROM
				obras2.arquivosupervisao ars
			INNER JOIN public.arquivo                     a ON a.arqid   = ars.arqid AND a.arqstatus = 'A'
                        LEFT  JOIN obras2.questaosupervisao          qs ON qs.sueid  = ars.sueid AND qs.qtsstatus = 'A'
                        LEFT  JOIN obras2.arquivoquestaosupervisao  aqs ON aqs.arqid = ars.arqid AND aqs.aqsstatus = 'A' AND aqs.qtsid = qs.qtsid
                        LEFT  JOIN obras2.respostasubquestao        rsq ON rsq.qtsid = qs.qtsid  AND rsq.rsqstatus = 'A'
                        LEFT  JOIN obras2.arquivorespostasubquestao ass ON ass.arqid = ars.arqid AND ass.arsstatus = 'A' AND ass.rsqid = rsq.rsqid
			WHERE
				(aqs.arqid IS NOT NULL OR ass.arqid IS NOT NULL) AND
				ars.aqsstatus = 'A' AND
				ars.sueid = {$sueid} AND
				ars.arqid = {$arqid}";
	$teste = $db->pegaUm($sql);

	return $teste;
}

function gravarFoto(){

	global $db;

	extract($_REQUEST);

	$sql = "INSERT INTO obras2.$tabela($campo, arqid) VALUES($id, $arqid)";

	$db->executar($sql);
	$db->commit();
}

function excluirFoto(){

	global $db;

	extract($_REQUEST);

	$status = $tabela == 'arquivoquestaosupervisao' ? 'aqsstatus' : 'arsstatus';
	$pkCampo 	= $tabela == 'arquivoquestaosupervisao' ? 'aqsid' : 'arsid';

	$sql = "UPDATE obras2.$tabela SET
				$status = 'I'
			WHERE
				$campo = $id
				AND $pkCampo = $pk
				AND arqid = $arqid";

	$db->executar($sql);
	$db->commit();
}

if( $_REQUEST['ajax'] ){
	switch ( $_REQUEST['ajax'] ){
		case 'buscaEndereco':
			require_once APPRAIZ . "adodb/adodb.inc.php";
			require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
			require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
			require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

			$endereco = new Endereco( $empreendimento->endid );

			$arEnd = array(
                                        'endcep' => $endereco->endcep,
                                        'endlogradouro' => $endereco->endlog,
                                        'endnum' => $endereco->endnum,
                                        'endcom' => $endereco->endcom,
                                        'endbai' => $endereco->endbai,
                                        'mundescricao' => $endereco->getMunDescricao(),
                                        'muncod' => $endereco->muncod,
                                        'estuf' => $endereco->estuf
                                      );

			$arEnd = array_map('utf8_encode',$arEnd);
			echo simec_json_encode( $arEnd );
			die;
		default:
			$_REQUEST['ajax']();
			die;

	}
}
$esdid = null;

if ( $_GET['acao'] == 'A' ){
	$sosid = ($_REQUEST['sosid'] ? $_REQUEST['sosid'] : $_SESSION['obras2']['sosid']);
//	$sosidHabil = ($sosid ? 'N' : 'S');
	$sueid 	                     = '';
	$_SESSION['obras2']['sueid'] = $sueid;
        $stsSup = true;
//	$endereco = new Endereco( $empreendimento->endid );
}
else{
    
//	$sosidHabil = ($sosid ? 'N' : 'S');
	$sosid                       = ($_REQUEST['sosid']       ?  $_REQUEST['sosid'] : $_SESSION['obras2']['sosid']);
	$sueid                       = ($_REQUEST['sueid'] != '' ? $_REQUEST['sueid']  : $_SESSION['obras2']['sueid']);
	$_SESSION['obras2']['sueid'] = ($_REQUEST['sueid'] != '' ? $_REQUEST['sueid']  : $_SESSION['obras2']['sueid']);

	if ( empty($sueid) ){
		die("<script>
                            alert('Faltam parametros para acessar a tela!');
                            window.location = '?modulo=principal/listVistoriaEmp&acao=A';
                     </script>");
	}

	$supervisaoEmpresa = new SupervisaoEmpresa( $sueid );

	extract( $supervisaoEmpresa->getDados() );
	$docid = pegaDocidSupervisaoEmpresa( $supervisaoEmpresa->sueid );
        $supEmpresa = $supervisaoEmpresa->pegaStatusSupEmpresa($docid);
        $stsSup = (WF_ESDID_LAUDO_SUPERVISAO_EM_CADASTRAMENTO == $supEmpresa['esdid']) ? true : false;
        //$endereco 		   = new Endereco( $endid );
        $status_sup = $supervisaoEmpresa->pegaStatusSupEmpresa($docid);
        $esdid = $status_sup['esdid'];
    
}

//esconde elementos
$display  = ($stsSup) ? '' : 'display:none;';
$cssClass = ($stsSup) ? 'originais' : '';

$sosidHabil = ($sosid ? 'N' : 'S');

// o EXTRACT limpa o empid, assim o recarrega
$empid = $_SESSION['obras2']['empid'];

// Executa as funções da tela de acordo com suas ações
if ($_REQUEST["requisicao"] == 'salvar' && $stsSup) {
    salvarSupervisaoEmpresa();
    die();
}

require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";



$endereco         = new Endereco( $endid ? $endid : $empreendimento->endid );
$somenteLeitura   = 'S';
$habilitado       = true;
$habilitadoPerfil = true;

//Demanda #230420
$perfis_empresa = array( PFLCOD_EMPRESA_CONTRATADA
                         ,PFLCOD_EMPRESA_VISTORIADORA_GESTOR
                         ,PFLCOD_EMPRESA_VISTORIADORA_FISCAL
                         ,PFLCOD_EMPRESA_MI_GESTOR
                         ,PFLCOD_EMPRESA_MI_FISCAL
                       );

if ($esdid == WF_ESDID_LAUDO_SUPERVISAO_HOMOLOGADO && !possuiPerfil(PFLCOD_SUPER_USUARIO) && possuiPerfil($perfis_empresa)){
    $habilitadoPerfil = false;
}elseif( possui_perfil( array(PFLCOD_EMPRESA_VISTORIADORA_FISCAL) ) ){
    if($entidvistoriador){
        $vistoriador = new Entidade( $entidvistoriador );
        if($_SESSION['usucpf'] != $vistoriador->entnumcpfcnpj){
            $habilitadoPerfil = false;
        }
    }
}elseif( possui_perfil( array(PFLCOD_GESTOR_MEC, PFLCOD_CALL_CENTER) ) ){
	$somenteLeitura 	= 'N';
	$habilitado     	= false;
	$habilitadoPerfil 	= false;
}

if(possui_perfil(PFLCOD_SUPER_USUARIO)){
    $habilitadoPerfil = true;
}

$os = new Supervisao_Os($sosid);

if($os->sosterreno == 't'){
   // $somenteLeitura 	= 'N';
   // $habilitado     	= false;
}

/**
 * se status da supervisão for != estid(cadastramento)
 * desabilita os forms
 */
if (!$stsSup) {
    ?>
    <script type="text/javascript">
        lock = function(){
            jQuery('textarea, input, select').not('[value=Voltar]').attr('disabled', true);
        }();
    </script>
    <?php
}


if( $_REQUEST['abaAjax'] ){
    
	$_SESSION['obras2']['abaAjax'] = $_REQUEST['abaAjax'];
	switch ( $_POST['abaAjax'] ){
		case 'dadosSupervisao':
			?>
                        <script src="/obras2/js/vistoria.js"></script>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%">
                        <tr>
		            <td class="SubTituloDireita" width="20%">OS</td>
		            <td>
		                <?php
				$situacaoObra = new Supervisao_Os();
		                $sql   	      = $situacaoObra->listaComboFiltroSupervisaoEmpresa( $empid, $sosid );
//		                $db->monta_combo("sosid", $sql, $sosidHabil, "Selecione...", "verificaObraParalisada", '', '', '', 'S', 'sosid');
		                $db->monta_combo("sosid", $sql, $sosidHabil, "Selecione...", "", '', '', '', 'S', 'sosid', false, $sosid);
		                ?>
		            </td>
		            <td rowspan="10" align="right" valign="top" width="1">
		            <?php
		            	// Barra de estado WORKFLOW
		            	if($docid){
                                    if( possui_perfil(PFLCOD_CALL_CENTER) ){
		                	wf_desenhaBarraNavegacao($docid, array('sueid' =>  $sueid), array('acoes' => true));
                                    }else{
		                	wf_desenhaBarraNavegacao($docid, array('sueid' =>  $sueid));
                                    }
                                }
		            ?>
		            </td>
		        </tr>

                <?php if ($dadosMi):     ?>
                <tr>
                    <td class="SubTituloDireita" width="20%">Nome da Construtora</td>
                    <td>
                        <input readonly="" type="text" size="40" name="" value="<?= $dadosMi['emidsc']?>" />
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="20%">CNPJ da Construtora</td>
                    <td>
                        <input readonly="" type="text" size="20" name="" value="<?= formatar_cnpj($dadosMi['emicnpj']) ?>" />
                    </td>
                </tr>
                <?php endif; ?>
		        <tr>
		            <td class="SubTituloDireita">Data da Supervisão</td>
		            <td>
		                <?php
		                echo campo_data2( 'suedtsupervisao', 'S', $somenteLeitura, '', 'S','','validaDataVistoria();', null, 'validaDataVistoria();');
		                ?>
		            </td>
		        </tr>
		        <tr>
		            <td class="SubTituloDireita">Nome do Responsável</td>
		            <td>
		                <?php
                                
//				  $vistoriador 		= new Entidade($vistoriaEstrutura['supvistoriador']);
				$vistoriador 		= new Entidade( $entidvistoriador );
		                $entnomevistoriador = $vistoriador->entnome;
		                $entidvistoriador   = $vistoriador->getPrimaryKey();
		                ?>
		                <span id="entnomevistoriador"><?php echo $entnomevistoriador; ?></span>
		                <input type="hidden" name="entidvistoriador" id="entidvistoriador" value="<?php echo $entidvistoriador; ?>">
		                <?php if($somenteLeitura == 'S' && $entnomevistoriador == ''){?>
		                <input type="button" name="pesquisar_entidade" value="Pesquisar" style="cursor: pointer;" onclick="inserirVistoriador(document.getElementById('entidvistoriador').value);"/>
		                <?php }?>
		                <?php print obrigatorio(); ?>
		            </td>
		        </tr>
		        <tr id="tr_cargo_vistoriador" style="display: <?php echo ($entidvistoriador ? '' : 'none' ) ?>">
		            <td class="SubTituloDireita">Cargo do Vistoriador</td>
		            <td>
				<?php echo campo_texto('suecargovistoriador', 'S', $somenteLeitura, '', 30, 255, '', '', '', '', 0, 'id="suecargovistoriador"' ); ?>
		            </td>
		        </tr>
		        <tr>
		            <td class="SubTituloDireita">Situação da Obra</td>
		            <td>
		                <?php
                                
				$situacaoObra = new SituacaoObra();
		                $sql = $situacaoObra->listaCombo();
		                $db->monta_combo("sobid", $sql, $somenteLeitura, "Selecione...", "ctrlSituacaoObra", '', '', '', 'S', 'sobid');
		                ?>
		            </td>
		        </tr>
                 <tr id="sueprojetofnde"  <?php if($sobid == 5){?> style="display:none;" <?php } ?>>
                    <td class="SubTituloDireita">Qual o projeto FNDE em execução?</td>
                    <td>
                        <input type="radio" name="sueprojetofnde" id="sueprojetofnde_s" value="c" <?php echo ($sueprojetofnde == 'c' ? 'checked="checked"' : '') ?> <?php echo ($habilitado ? '' : 'disabled="disabled"') ?>>
                        <label for="sueprojetofnde_s">Convencional</label>
                        &nbsp;&nbsp;
                        <input type="radio" name="sueprojetofnde" id="sueprojetofnde_n" value="m" <?php echo ($sueprojetofnde == 'm' ? 'checked="checked"' : '') ?> <?php echo ($habilitado ? '' : 'disabled="disabled"') ?>>
                        <label for="sueprojetofnde_n">Metodologia inovadora</label>
                        <?php echo obrigatorio(); ?>
                    </td>
                </tr>        
                        
                        
                <tr>
                    <td class="SubTituloDireita">O terreno está em área urbana ou rural?</td>
                    <td>
                        <input type="radio" name="sueruralurbano" id="sueruralurbano_s" value="u" <?php echo ($sueruralurbano == 'u' ? 'checked="checked"' : '') ?> <?php echo ($habilitado ? '' : 'disabled="disabled"') ?>>
                        <label for="sueruralurbano_s">Urbana</label>
                        &nbsp;&nbsp;
                        <input type="radio" name="sueruralurbano" id="sueruralurbano_n" value="r" <?php echo ($sueruralurbano == 'r' ? 'checked="checked"' : '') ?> <?php echo ($habilitado ? '' : 'disabled="disabled"') ?>>
                        <label for="sueruralurbano_n">Rural</label>
                        <?php echo obrigatorio(); ?>
                    </td>
                </tr>

                        
                        
                        <tr id="tr-paralisacao" <?php if($sobid != 4){?> style="display:none;" <?php } ?>>
		            <td class="SubTituloDireita">Tipo de paralisação</td>
		            <td>
		                <?php
                                //TIpo de paralisação
				$situacaoObra = new TipoParalisacao();
		                $sql = $situacaoObra->listaCombo();
		                $db->monta_combo("tplid", $sql, $somenteLeitura, "Selecione...", "", '', '', '', 'S', 'tplid');
		                ?>
		            </td>
		        </tr>
                     
		        <tr>
		            <td class="SubTituloDireita">Observação</td>
		            <td>
                        <?php $sueobservacao = stripslashes($sueobservacao)?>
		                <?= campo_textarea('sueobservacao', 'S', 'S', '', '70', '8', '300', '', 0, '', '', false, '', ''); ?>
		            </td>
		        </tr>
                       

                <?php if ($qstescopo == 'SE'): ?>

		        <tr id="situacaoObraSubPergunta" style="display: <?php echo ( $sobid == SITUACAO_OBRA_EM_EXECUCAO ? '' : 'none' ) ?>;">
		            <td class="SubTituloDireita">Em conformidade com o Projeto Básico aprovado e contrato?</td>
		            <td>
		            	<input type="radio" name="sueacordo" id="sueacordo_s" value="s" <?php echo ($sueacordo == 's' ? 'checked="checked"' : '') ?> <?php echo ($habilitado ? '' : 'disabled="disabled"') ?>>
		            	<label for="sueacordo_s">Sim</label>
		            	&nbsp;&nbsp;
		            	<input type="radio" name="sueacordo" id="sueacordo_n" value="n" <?php echo ($sueacordo == 'n' ? 'checked="checked"' : '') ?> <?php echo ($habilitado ? '' : 'disabled="disabled"') ?>>
		            	<label for="sueacordo_n">Não</label>
		            	<?php echo obrigatorio(); ?>
		            </td>
		        </tr>
                <tr>
                    <td class="SubTituloDireita">Total atual do percentual de execução da obra</td>
                    <td>

                        <?php
                        $suepercentualexe = str_replace('.',',', $suepercentualexe);
                        echo campo_texto('suepercentualexe', 'N', 'S', '', 11, 30, '###,##', '', 'right', '', 0, '');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Memória de Cálculo do percentual total</td>
                    <td>
                        <?php
                        if(!empty($suearqmemcalc)){
                            $arquivo = new Arquivo($suearqmemcalc);
                        }
                        ?>
                        <input type="file" name="arquivo" id="arquivo"/>
                        <br>
                        <?php
                        if ($arquivo->arqid) {
                            echo "
                                <a href='?modulo=principal/popupAceiteOS&acao=A&requisicao=downloadArquivo&arqid={$arquivo->arqid}'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>
                                ";
                        }
                        ?>
                    </td>
                </tr>
                <?php else: ?>
                    <tr id="total_percentual" style="display: <?php echo ( $sobid != 5 ? '' : 'none' ) ?>;">
                        <td class="SubTituloDireita">Total atual do percentual de execução da obra</td>
                        <td>

                            <?php 
                            $suepercentualexe = str_replace('.',',', $suepercentualexe);
                            echo campo_texto('suepercentualexe', 'N', 'S', '', 11, 30, '###,##', '', 'right', '', 0, '');
                            ?>
                        </td>
                    </tr>
                <?php endif; ?>
                <tr id="unidade_funcionamento" style="display: <?php echo ( $sobid != 5 ? '' : 'none' ) ?>;">
                    <td class="SubTituloDireita">Unidade em Funcionamento?</td>
                    <td>
                        <input type="radio" name="suefuncionamento" id="suefuncionamento_s" value="s" <?php echo ($suefuncionamento == 's' ? 'checked="checked"' : '') ?> <?php echo ($habilitado ? '' : 'disabled="disabled"') ?>>
                        <label for="suefuncionamento_s">Sim</label>
                        &nbsp;&nbsp;
                        <input type="radio" name="suefuncionamento" id="suefuncionamento_n" value="n" <?php echo ($suefuncionamento == 'n' ? 'checked="checked"' : '') ?> <?php echo ($habilitado ? '' : 'disabled="disabled"') ?>>
                        <label for="suefuncionamento_n">Não</label>
                        <?php echo obrigatorio(); ?>
                    </td>
                </tr>

                <?php
                    $obj_os = new Supervisao_Os($sosid);
                    
                    
                    if($obj_os->sostipo == 'R'){
                        
                 
                       
                            
                ?>
                    <tr>
                        <td class="SubTituloDireita" width="30%" nowrap="nowrap">Observação de retificação de OS: </td>
                        <td>
                            <?php
                                $sueobsretificacaoos = empty($sueobsretificacaoos) ? '' : $sueobsretificacaoos;
                                echo campo_textarea('sueobsretificacaoos', 'S', 'S', '', '70', '8', '255', '', 0, '', '', false, $sueobsretificacaoos);
                            ?>

                        </td>

                    </tr>
                <?php 
                
                    } ?>

                <tr>
                    <?php
                    
                    
                    if(obraMI($obra->obrid)):
                        
                        
                        $ordemServico = new OrdemServicoMI();
                        $dados = $ordemServico->carregarPorObridETomid($obra->obrid, 3);
                        
                        if(!empty($dados)){
                        $anexoOsMi = new AnexoOsMi();
                        $anexoOsMi = $anexoOsMi->getAnexoExecucaoImplantacao($dados['osmid']);
                        }
                        
                    
                        
                        
                        if (!empty($anexoOsMi) > 0) :
                        ?>
                        <td class="SubTituloDireita">Projeto de Implantação</td>
                        <td>
                            <?php
                            foreach ($anexoOsMi as $anexo) {
                                $arquivo = new Arquivo($anexo['arqid']);
                                if(empty($anexo['arqid']))continue;
                                ?>
                                <p>
                                    <?php
                                    echo "<a href='?modulo=principal/cadVistoriaEmpresa&acao=A&requisicao=downloadArquivo&arqid={$arquivo->arqid}'>  (" . $anexo['taonome'] . ") " . $arquivo->arqnome . "." . $arquivo->arqextensao . " </a>";
                                    ?>
                                </p>
                            <?php } ?>
                        </td>
                        <?php endif;?>
                    <?php endif;?>
                </tr>
                <tr>
                    <td colspan="2" align="center">
                        <div id="divQuestionarioObraParalisada" style="display: none">
                            <?php conteudoDivQuestionarioObraParalisada('tela'); ?>
                        </div>
                    </td>
                </tr>
            </table>
			<?php
			die;
		case 'localObra':
			?>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%" >
				<?php
//				if ( $sueendcorreto == 's' || empty($suefuncionamento)){
				if ($sueendcorreto == 's' || empty($sueendcorreto) || $habilitado == false){
					$campoEndHabil = 'N';
					$campoEndClass = 'disabled';
				} else {
					$campoEndHabil = 'S';
					$campoEndClass = '';
				}
				?>
				<tr>
					<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>No cadastro da obra o endereço está correto?</label></td>
					<td colspan="1">
                                            <input type="radio" onclick="verificaEnd('S')" name="sueendcorreto" id="sueendcorreto_s" value="s" <?php echo ($sueendcorreto == 's' || empty($sueendcorreto) ? 'checked="checked"' : '') ?> <?php echo ($habilitado ? '' : 'disabled="disabled"') ?>>
                                            <label for="sueendcorreto_s">Sim</label>
                                            &nbsp;&nbsp;
                                            <input type="radio" onclick="verificaEnd('N')" name="sueendcorreto" id="sueendcorreto_n" value="n" <?php echo ($sueendcorreto == 'n' ? 'checked="checked"' : '') ?> <?php echo ($habilitado ? '' : 'disabled="disabled"') ?>>
                                            <label for="sueendcorreto_n">Não</label>
                                            <?php echo obrigatorio(); ?>
					</td>
				</tr>
				<tr>
					<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>CEP</label></td>
					<td colspan="1">
						<input type="hidden" name="emp_endid" id="emp_endid" value="<?php echo $empreendimento->endid ?>">
						<input type="hidden" name="endid" id="endid" value="<?php echo $endereco->endid ?>">
						<input type="text" name="endereco[endcep]"
							   onkeyup="this.value=mascaraglobal('##.###-###', this.value);"
							   onblur="Entidade.__getEnderecoPeloCEP(this);" class="CampoEstilo <?php echo $campoEndClass?>" id="endcep"
							   value="<?php echo $endereco->endcep ?>" size="13" maxlength="10"
							   <?php echo $campoEndHabil == 'N' ? 'readonly="readonly"' : '' ?>/>
						<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
					</td>
				</tr>
				<tr id="escolha_logradouro_id" style="display:none">
					<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Sugestão de Logradouro:</label></td>
					<td colspan="1">
						<input type="text" name="endlog" class="CampoEstilo" id="endlog" value="<?php echo $endereco->endlog ?>" size="48" />
					</td>
				</tr>
				<tr>
					<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
		            	<label id="lbLogadouro"> Logradouro </label>
		            </td>
		            <td colspan="1">
		           		<input type="text" name="endereco[endlog]" class="CampoEstilo <?php echo $campoEndClass?>" id="endlogradouro" value="<?php echo $endereco->endlog ?>" size="65" <?php echo $campoEndHabil == 'N' ? 'readonly="readonly"' : '' ?> />
		      		</td>
				</tr>
				<tr>
					<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Número</label></td>
		      		<td colspan="2">
		  				<input type="text" name="endereco[endnum]" class="CampoEstilo <?php echo $campoEndClass?>" id="endnum" value="<?php echo $endereco->endnum ?>" size="13" maxlength="8" <?php echo $campoEndHabil == 'N' ? 'readonly="readonly"' : '' ?> />
		            </td>
		      	</tr>
				<tr>
					<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Complemento</label></td>
		      		<td colspan="2">
		           		<input type="text" name="endereco[endcom]" class="CampoEstilo <?php echo $campoEndClass?>" id="endcom" value="<?php echo $endereco->endcom ?>" size="65" maxlength="100" <?php echo $campoEndHabil == 'N' ? 'readonly="readonly"' : '' ?> />
		          	</td>
		    	</tr>
				<tr>
		         	<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Bairro</label></td>
		     		<td colspan="2">
		    			<input type="text" name="endereco[endbai]" class="CampoEstilo <?php echo $campoEndClass?>" id="endbai" value="<?php echo $endereco->endbai ?>" size="20" <?php echo $campoEndHabil == 'N' ? 'readonly="readonly"' : '' ?> />
		        	</td>
		    	</tr>
		    	<tr>
		          	<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Município/UF </label></td>
		          	<td colspan="2">
		              	<input type="text" name="endereco[mundescricao]"  class="CampoEstilo <?php echo $campoEndClass?>" id="mundescricao" value="<?php echo $endereco->getMunDescricao()?>" size="20" <?php echo $campoEndHabil == 'N' ? 'readonly="readonly"' : '' ?> />
		              	<input type="hidden" name="endereco[muncod]" id="muncod" class="CampoEstilo" value="<?php echo $endereco->muncod ?>" />
		             	<input type="text" name="endereco[estuf]" class="CampoEstilo <?php echo $campoEndClass?>" id="estuf" value="<?php echo $endereco->estuf ?>" style="width: 5ex; padding-left: 2px" <?php echo $campoEndHabil == 'N' ? 'readonly="readonly"' : '' ?> />
		         	</td>
		      	</tr>
	      	</table>
			<?php
			die;
                case 'cronograma':
                ?>
                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%" >
                <?php
                        $param   = array();
                        $param['not(obridpai)'] = true;
                        $obras   = new Obras();
                        $arObrid = $obras->pegaIdObraEVinculadasPorEmpid($empid, $param);

//                        $arObrid = array($_SESSION['obras2']['obrid']);
//                        var_dump($_SESSION['obras2']['obrid']);

                        // Tratamento para quando a obra for vinculada
                        $arObridVinculada = array();
                        $arObridAtiva = array();
                        foreach ($arObrid as $obrid) {
                            $supervisao         = new Supervisao();
                            $supid              = $supervisao->pegaSupidByObraAndSueid($obrid, $sueid);
                            $ultimoSupid        = $supervisao->pegaUltSupidByObra($obrid, $param);
                            $obras              = new Obras($obrid);
                            if (!empty($supid) && $obras->obrstatus == 'P') {
                                $arObridVinculada = array($obrid);
                            } else if ( $obras->obrstatus == 'A') {
                                $arObridAtiva = array($obrid);
                            }
                        }
                        $arObrid = (!empty($arObridVinculada)) ? $arObridVinculada : $arObridAtiva;

                        $obras   = new Obras();

                        foreach ($arObrid as $obrid) {

                            $obra               = new Obras($obrid);
                            $obraMi = ($obra->tpoid == TPOID_MI_TIPO_B || $obra->tpoid == TPOID_MI_TIPO_C) ? true : false;
                            $param              = array();
                            $param['empid']     = $empid;
                            $param['is(sueid)'] = true;
                            $supervisao         = new Supervisao();
                            $supid              = $supervisao->pegaSupidByObraAndSueid($obrid, $sueid);
                            $ultimoSupid        = $supervisao->pegaUltSupidByObra($obrid, $param);
                            unset($total);
                ?>
                        <tr id="tr9" >
                            <td colspan="3">
                                <table class="listagem" width="100%" bgcolor="#FFFFFF" id="lista_supervisao">
                                    <thead>
                                        <tr style="background-color: #CDCDCD;">
                                            <td colspan="15" valign="middle" align="left" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                                <input type="hidden" name="obrid[]" id="obrid_<?php echo $obrid ?>" value="<?php echo $obrid ?>">
                                                <input type="hidden" name="supid[<?php echo $obrid ?>]" id="supid_<?php echo $obrid ?>" value="<?php echo $supid ?>">
                                                <b><?php echo "($obrid) {$obra->obrnome}" ?></b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Descrição</b></td>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Valor (R$)</b></td>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) Sobre o Objeto</b></td>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Quantidade</b></td>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Unidade de Medida</b></td>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Data de Início</b></td>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Data de Término</b></td>
                                            <td colspan="2" rowspan="1" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title" ><b>Última Supervisão</b></td>
                                            <td colspan="4" rowspan="1" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title" ><b>Supervisão Atual</b></td>
                                            <td colspan="2" rowspan="1" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title" ><b>Diferença para a Supervisão anterior</b></td>
                                        </tr>
                                        <tr>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado</b></td>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado sobre o Objeto</b></td>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) Supervisão</b></td>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Valor Executado<br></b></td>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado sobre o Objeto após Supervisão</b></td>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Quantidade Executada<br></b></td>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Valor Executado</b></td>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) Executado<br></b></td>
                                        </tr>
                                    </thead>

                                <?php
                                $supervisaoItem = new SupervisaoItem();

                                $total['vlrProjeto']           = 0;
                                $total['vlrObra']              = 0;
                                $total['vlrExecSobreObra']     = 0;
                                $total['vlrExecSobreObraUlt']  = 0;
                                $total['percExecSobreObraUlt'] = 0;

//                                var_dump($supid);
//                                var_dump($ultimoSupid);


                                // Busca filhos da Edificação
                                $dadoEtapa  = $supervisaoItem->getItensByEtapa($obrid, ($supid ? $supid : $ultimoSupid));
                                $dadoEtapaF = $supervisaoItem->getItensByEtapa($obrid, ($supid ? $supid : $ultimoSupid), 'F');

                                $habil      = (count($dadoEtapa) ? 'N' : 'S');

                                foreach ($dadoEtapa as $etapa) {

                                    $total['vlrProjeto'] = $etapa['ocrvalorexecucao'];
                                    $total['vlrObra'] += $etapa['icovlritem'];
                                    $total['percObra'] = ($etapa['ocrvalorexecucao'] > 0 ? ($total['vlrObra'] / $etapa['ocrvalorexecucao']) * 100 : 0);
                                    $total['vlrExecSobreObra'] += $etapa['spivlrfinanceiroinfsupervisor'];
                                    $total['percExecSobreObra'] = ($etapa['ocrvalorexecucao'] > 0 ? ($total['vlrExecSobreObra'] / $etapa['ocrvalorexecucao']) * 100 : 0);
                                    $total['vlrExecSobreObraUlt'] += $etapa['spivlrfinanceiroanterior'];
                                    $total['percExecSobreObraUlt'] = ($etapa['ocrvalorexecucao'] > 0 ? ($total['vlrExecSobreObraUlt'] / $etapa['ocrvalorexecucao']) * 100 : 0);
                                    // $total['percExecSobreObraUlt'] += $edificacao['supvlritemsobreobraexecanterior'];

                                    $etapa['icodtinicioitem'] = formata_data($etapa['icodtinicioitem']);
                                    $etapa['icodterminoitem'] = formata_data($etapa['icodterminoitem']);
                                    $etapa['spipercsobreobrainfsupervisor'] = ($etapa['ocrvalorexecucao'] > 0 ? ($etapa['spivlrfinanceiroinfsupervisor'] / $etapa['ocrvalorexecucao']) * 100 : 0);
                                    // Busca Filhos da Etapa
                                    $dadoDetalhe = $supervisaoItem->getItensByDetalhamento($obrid, $etapa['icoid'], ($supid ? $supid : $ultimoSupid), array("di.ditidpai IS NULL"));
                                    $habil = ( (count($dadoDetalhe) || $habilitado == false) ? 'N' : 'S');
                                ?>
                                        <tbody>
                                            <tr bgcolor="#FDF8E7" id="<?php echo "tr_item_etapa_" . $obrid . "_" . $etapa['icoid'] ?>">
                                                <td align="left">
                                                    <input type="hidden" name="icoid[<?php echo $obrid ?>][]" value="<?php echo $etapa['icoid'] ?>">
                                                    <input type="hidden" name="ico_spiid[<?php echo $obrid ?>][]" value="<?php echo $etapa['spiid'] ?>">
                                                    <input type="hidden" name="ico_spitotitemsupervisor[<?php echo $obrid ?>][]" value="">
                                                    &nbsp;<img src="/imagens/seta_filho.gif" border="0"><?php echo $etapa['itcdesc'] ?>
                                                </td>
                                                <td align="right" style="color:#336EFF">
                                                    <?php echo number_format($etapa['icovlritem'], 2, ',', '.') ?>
                                                </td>
                                                <td align="right" style="color:#336EFF">
                                                    <?php echo number_format($etapa['icopercsobreobra'], 2, ',', '.') ?>
                                                </td>
                                                <td align="right" style="color:#336EFF">
                                                -
                                                </td>
                                                <td align="center">
                                                -
                                                </td>
                                                <td align="center">
                                                    <?php echo $etapa['icodtinicioitem'] ?>
                                                </td>
                                                <td align="center">
                                                    <?php echo $etapa['icodterminoitem'] ?>
                                                </td>
                                                <td align="center">
                                                    <input type="hidden" name="ico_spivlrfinanceiroanterior[]" value="<?php echo number_format($etapa['spivlrfinanceiroanterior'], 2, ',', '.') ?>">
                                                    <?php echo campo_texto('ico_spivlritemexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($etapa['spivlritemexecanterior'], 2, ',', '.')); ?>
                                                </td>
                                                <td align="center">
                                                    <?php echo campo_texto('ico_spivlritemsobreobraexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($etapa['spivlritemsobreobraexecanterior'], 2, ',', '.')); ?>
                                                </td>
                                                <td align="center">
                                                    <?php echo campo_texto('ico_spivlrinfsupervisor[' . $obrid . '][]', 'N', $habil, '', 7, 6, '', '', 'right', '', 0, '', 'calculoSupervisao.managerEtapa( this, ' . $obrid . ' )', number_format($etapa['spivlrinfsupervisor'], 2, ',', '.')); ?>
                                                </td>
                                                <td align="center">
                                                    <?php echo campo_texto('ico_spivlrfinanceiroinfsupervisor[' . $obrid . '][]', 'N', $habil, '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerEtapa( this, ' . $obrid . ', \'numerico\' )', number_format($etapa['spivlrfinanceiroinfsupervisor'], 2, ',', '.')); ?>
                                                </td>
                                                <td align="right" style="color:#336EFF">
                                                    <input type="hidden" name="ico_spipercsobreobrainfsupervisor[<?php echo $obrid ?>][]" value="<?php echo number_format($etapa['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>">
                                                    <span>
                                                        <?php echo number_format($etapa['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>
                                                    </span>
                                                </td>
                                                <td align="right" style="color:#336EFF">
                                                &nbsp;
                                                </td>
                                                <td align="right" style="color:#336EFF">
                                                    <input type="hidden" value="<?php echo number_format($etapa['spivlrfinanceiroinfsupervisor'] - $etapa['spivlrfinanceiroanterior'], 2, ',', '.'); ?>" name="ico_spidiferencavlr[]">
                                                    <span> <?php echo number_format($etapa['spivlrfinanceiroinfsupervisor'] - $etapa['spivlrfinanceiroanterior'], 2, ',', '.'); ?> </span>
                                                </td>
                                                <td align="right" style="color:#336EFF">
                                                    <input type="hidden" value="<?php echo number_format($etapa['spivlrinfsupervisor'] - $etapa['spivlritemexecanterior'], 2, ',', '.'); ?>" name="ico_spidiferencaporcentagem[]">
                                                    <span> <?php echo number_format($etapa['spivlrinfsupervisor'] - $etapa['spivlritemexecanterior'], 2, ',', '.'); ?> </span>
                                                </td>
                                            </tr>
                                        </tbody>
                                    <?php

                                    foreach ($dadoDetalhe as $detalhe) {
                                        $ditid = $detalhe['ditid'];
                                        $detalhe['ditdatainicio'] = formata_data($detalhe['ditdatainicio']);
                                        $detalhe['ditdatatermino'] = formata_data($detalhe['ditdatatermino']);
                                        $detalhe['spipercsobreobrainfsupervisor'] = ($detalhe['spivlrfinanceiroinfsupervisor'] / $detalhe['ocrvalorexecucao']) * 100;

                                        //adicionando os filhos
                                        $dadoDetalheFilho = $supervisaoItem->getItensByDetalhamento($obrid, $etapa['icoid'], ($supid ? $supid : $ultimoSupid), array("di.ditidpai ={$ditid}"));
                                        $habil = ( (count($dadoDetalheFilho) || $habilitado == false) ? 'N' : 'S');
                                        ?>
                                                        <tbody>
                                                            <tr bgcolor="#DBF6D5" id="<?php echo "tr_item_detalhamento_" . $obrid . "_" . $detalhe['icoid'] . "_" . $detalhe['ditid'] ?>">
                                                                <td align="left">
                                                                <input type="hidden" name="ditid[<?php echo $obrid ?>][]" value="<?php echo $detalhe['ditid'] ?>">
                                                                <input type="hidden" name="dit_spiid[<?php echo $obrid ?>][]" value="<?php echo $detalhe['spiid'] ?>">
                                                                &nbsp;&nbsp;&nbsp;&nbsp;<img src="/imagens/seta_filho.gif" border="0"><?php echo $detalhe['ditdsc'] ?>
                                                                </td>
                                                                <td align="right" style="color:#336EFF">
                                        <?php echo number_format($detalhe['ditvalor'], 2, ',', '.') ?>
                                                                </td>
                                                                <td align="right" style="color:#336EFF">
                                        <?php echo number_format($detalhe['ditpercsobreobra'], 2, ',', '.') ?>
                                                                </td>
                                                                <td align="right" style="color:#336EFF">
                                        <?php echo ($detalhe['ditmetafisica'] ? number_format($detalhe['ditmetafisica'], 2, ',', '.') : '-') ?>
                                                                </td>
                                                                <td align="center">
                                        <?php echo ($detalhe['umcdsc'] ? $detalhe['umcdsc'] : '-') ?>
                                                                </td>
                                                                <td align="center">
                                        <?php echo $detalhe['ditdatainicio'] ?>
                                                                </td>
                                                                <td align="center">
                                        <?php echo $detalhe['ditdatatermino'] ?>
                                                                </td>
                                                                <td align="center">
                                                                <input type="hidden" name="dit_spivlrfinanceiroanterior[]" value="<?php echo number_format($detalhe['spivlrfinanceiroanterior'], 2, ',', '.') ?>">
                                        <?php echo campo_texto('dit_spivlritemexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($detalhe['spivlritemexecanterior'], 2, ',', '.')); ?>
                                                                </td>
                                                                <td align="center">
                                        <?php echo campo_texto('dit_spivlritemsobreobraexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($detalhe['spivlritemsobreobraexecanterior'], 2, ',', '.')); ?>
                                                                </td>
                                                                <td align="center">
                                        <?php echo campo_texto('dit_spivlrinfsupervisor[' . $obrid . '][]', 'N', $habil, '', 7, 6, '', '', 'right', '', 0, '', 'calculoSupervisao.managerDetalhamento( this, ' . $obrid . ', ' . $detalhe['icoid'] . ' )', number_format($detalhe['spivlrinfsupervisor'], 2, ',', '.')); ?>


                                                                </td>
                                                                <td align="center">
                                        <?php echo campo_texto('dit_spivlrfinanceiroinfsupervisor[' . $obrid . '][]', 'N', $habil, '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerDetalhamento( this, ' . $obrid . ', ' . $detalhe['icoid'] . ', \'numerico\' )', number_format($detalhe['spivlrfinanceiroinfsupervisor'], 2, ',', '.')); ?>
                                                                </td>
                                                                <td align="right" style="color:#336EFF">
                                                                <input type="hidden" name="dit_spipercsobreobrainfsupervisor[<?php echo $obrid ?>][]" value="<?php echo number_format($detalhe['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>">
                                                                <span>
                                        <?php echo number_format($detalhe['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>
                                                                </span>
                                                                </td>
                                                                <td align="right" style="color:#336EFF">
                                        <?php
                                        echo ($detalhe['umcdsc'] ?
                                                campo_texto('dit_spivlrmetafisicasupervisor[' . $detalhe['ditid'] . '][]', 'N', 'S', '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerMetaFisica( this, ' . $detalhe['ditmetafisica'] . ' )', number_format($detalhe['spivlrmetafisicasupervisor'], 2, ',', '.')) :
                                                '&nbsp;')
                                        ?>
                                                                </td>

                                                            <td align="right" style="color:#336EFF">
                                                                        <input type="hidden" value="<?php echo number_format($etapa['spivlrfinanceiroinfsupervisor'] - $detalhe['spivlrfinanceiroanterior'], 2, ',', '.'); ?>" name="dit_spidiferencavlr[]">
                                                                                                        <span> <?php echo number_format($detalhe['spivlrfinanceiroinfsupervisor'] - $detalhe['spivlrfinanceiroanterior'], 2, ',', '.'); ?> </span>
                                                                </td>
                                                                <td align="right" style="color:#336EFF">
                                                                        <input type="hidden" value="<?php echo number_format($detalhe['spivlrinfsupervisor'] - $detalhe['spivlritemexecanterior'], 2, ',', '.'); ?>" name="dit_spidiferencaporcentagem[]">
                                                                                                        <span> <?php echo number_format($detalhe['spivlrinfsupervisor'] - $detalhe['spivlritemexecanterior'], 2, ',', '.'); ?> </span>
                                                                </td>
                                                            </tr>
                                                        </tbody>
                                        <?php
                                        foreach ($dadoDetalheFilho as $detalheFilho) {
                                            $detalheFilho['ditdatainicio'] = formata_data($detalheFilho['ditdatainicio']);
                                            $detalheFilho['ditdatatermino'] = formata_data($detalheFilho['ditdatatermino']);

                                            $detalheFilho['spipercsobreobrainfsupervisor'] = ($detalheFilho['spivlrfinanceiroinfsupervisor'] / $detalheFilho['ocrvalorexecucao']) * 100;
                                            ?>
                                                            <tbody>
                                                                <tr bgcolor="#D7EAF2" id="<?php echo "tr_item_detalhamento_filho_" . $obrid . "_" . $detalheFilho['icoid'] . "_" . $detalheFilho['ditidpai'] . "_" . $detalheFilho['ditid'] ?>">
                                                                    <td align="left">
                                                                    <input type="hidden" name="ditid_filho[<?php echo $obrid ?>][]" value="<?php echo $detalheFilho['ditid'] ?>">
                                                                    <input type="hidden" name="dtf_spiid[<?php echo $obrid ?>][]" value="<?php echo $detalheFilho['spiid'] ?>">
                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="/imagens/seta_filho.gif" border="0"><?php echo $detalheFilho['ditdsc'] ?>
                                                                    </td>
                                                                    <td align="right" style="color:#336EFF">
                                            <?php echo number_format($detalheFilho['ditvalor'], 2, ',', '.') ?>
                                                                    </td>
                                                                    <td align="right" style="color:#336EFF">
                                            <?php echo number_format($detalheFilho['ditpercsobreobra'], 2, ',', '.') ?>
                                                                    </td>
                                                                    <td align="right" style="color:#336EFF">
                                            <?php echo ($detalheFilho['ditmetafisica'] ? number_format($detalheFilho['ditmetafisica'], 2, ',', '.') : '-') ?>
                                                                    </td>
                                                                    <td align="center">
                                            <?php echo ($detalheFilho['umcdsc'] ? $detalheFilho['umcdsc'] : '-') ?>
                                                                    </td>
                                                                    <td align="center">
                                            <?php echo $detalheFilho['ditdatainicio'] ?>
                                                                    </td>
                                                                    <td align="center">
                                            <?php echo $detalheFilho['ditdatatermino'] ?>
                                                                    </td>
                                                                    <td align="center">
                                                                    <input type="hidden" name="dtf_spivlrfinanceiroanterior[]" value="<?php echo number_format($detalheFilho['spivlrfinanceiroanterior'], 2, ',', '.') ?>">
                                            <?php echo campo_texto('dtf_spivlritemexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($detalheFilho['spivlritemexecanterior'], 2, ',', '.')); ?>
                                                                    </td>
                                                                    <td align="center">
                                            <?php echo campo_texto('dtf_spivlritemsobreobraexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($detalheFilho['spivlritemsobreobraexecanterior'], 2, ',', '.')); ?>
                                                                    </td>
                                                                    <td align="center">
                                            <?php echo campo_texto('dtf_spivlrinfsupervisor[' . $obrid . '][]', 'N', 'S', '', 7, 6, '', '', 'right', '', 0, '', 'calculoSupervisao.managerDetalhamentoFilho( this, ' . $obrid . ', ' . $detalheFilho['icoid'] . ', ' . $detalheFilho['ditidpai'] . ' )', number_format($detalheFilho['spivlrinfsupervisor'], 2, ',', '.')); ?>


                                                                    </td>
                                                                    <td align="center">
                                            <?php echo campo_texto('dtf_spivlrfinanceiroinfsupervisor[' . $obrid . '][]', 'N', 'S', '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerDetalhamentoFilho( this, ' . $obrid . ', ' . $detalheFilho['icoid'] . ', ' . $detalheFilho['ditidpai'] . ', \'numerico\'  )', number_format($detalheFilho['spivlrfinanceiroinfsupervisor'], 2, ',', '.')); ?>
                                                                    </td>
                                                                    <td align="right" style="color:#336EFF">
                                                                    <input type="hidden" name="dtf_spipercsobreobrainfsupervisor[<?php echo $obrid ?>][]" value="<?php echo number_format($detalheFilho['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>">
                                                                    <span>
                                            <?php echo number_format($detalheFilho['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>
                                                                    </span>
                                                                    </td>
                                                                    <td align="right" style="color:#336EFF">
                                            <?php
                                            echo ($detalheFilho['umcdsc'] ?
                                                    campo_texto('dtf_spivlrmetafisicasupervisor[' . $obrid . '][' . $detalheFilho['ditid'] . '][]', 'N', 'S', '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerMetaFisica( this, ' . $detalheFilho['ditmetafisica'] . ' )', number_format($detalheFilho['spivlrmetafisicasupervisor'], 2, ',', '.')) :
                                                    '&nbsp;')
                                            ?>
                                                                        </td>

                                                                        <td align="right" style="color:#336EFF">
                                                                            <input type="hidden" value="<?php echo number_format($detalheFilho['spivlrfinanceiroinfsupervisor'] - $detalheFilho['spivlrfinanceiroanterior'], 2, ',', '.'); ?>" name="dtf_spidiferencavlr[]">
                                                                            <span> <?php echo number_format($detalheFilho['spivlrfinanceiroinfsupervisor'] - $detalheFilho['spivlrfinanceiroanterior'], 2, ',', '.'); ?> </span>
                                                                        </td>
                                                                        <td align="right" style="color:#336EFF">
                                                                            <input type="hidden" value="<?php echo number_format($detalheFilho['spivlrinfsupervisor'] - $detalheFilho['spivlritemexecanterior'], 2, ',', '.'); ?>" name="dtf_spidiferencaporcentagem[]">
                                                                            <span> <?php echo number_format($detalheFilho['spivlrinfsupervisor'] - $detalheFilho['spivlritemexecanterior'], 2, ',', '.'); ?> </span>
                                                                        </td>

                                                                    </tr>
                                                            </tbody>
                                            <?php
                                        }
                                    }
                                }
                                ?>
                                    <tbody>
                                        <tr id="tr_total_supervisao_<?php echo $obrid ?>" bgcolor="#E3E3E3">
                                            <td align="right">
                                            <b>Total</b>
                                            <input type="hidden" name="vlrProjeto_<?php echo $obrid ?>" id="vlrProjeto_<?php echo $obrid ?>" value="<?php echo $total['vlrProjeto'] ?>">
                                            </td>
                                            <td align="right" style="color:#336EFF">
                                <?php echo number_format($total['vlrObra'], 2, ',', '.') ?>
                                            </td>
                                            <td align="right" style="color:#336EFF">
                                <?php echo number_format($total['percObra'], 2, ',', '.') ?>
                                            </td>
                                            <td align="center">
                                            &nbsp;
                                            </td>
                                            <td align="center">
                                            &nbsp;
                                            </td>
                                            <td align="center">
                                            &nbsp;
                                            </td>
                                            <td align="center">
                                            &nbsp;
                                            </td>
                                            <td align="right">
                                            &nbsp;
                                <?php // echo campo_texto( 'total_spivlritemexecanterior', 'N', 'N', '', 7, 6, '', '', '', '', 0, '', '', '0,00'); ?>
                                            </td>
                                            <td align="center">
                                <?php echo campo_texto('total_spivlritemsobreobraexecanterior', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($total['percExecSobreObraUlt'], 2, ',', '.')); ?>
                                            </td>
                                            <td align="right">
                                <?php // echo campo_texto( 'total_spivlrinfsupervisor', 'N', 'N', '', 7, 6, '', '', '', '', 0, '', '', '0,00'); ?>
                                            </td>
                                            <td align="right">
                                            &nbsp;
                                            </td>
                                            <td align="right" style="color:#336EFF">
                                <?php echo number_format($total['percExecSobreObra'], 2, ',', '.') ?>
                                            </td>
                                            <td align="right">
                                            &nbsp;
                                            </td>
                                            <td align="right">
                                            &nbsp;
                                            </td>
                                            <td align="right">
                                            &nbsp;
                                            </td>
                                        </tr>
                                    </tbody>
                                </table>
                                <br>
                            </td>
                        </tr>


                    <?php if($obraMi): ?>
                    <tr>
                        <td colspan="3" bgcolor="#DCDCDC" style="text-align: center; font-weight: bold">
                            Serviços externos
                        </td>
                    </tr>
                    <tr>
                    <td colspan="3">
                        <table class="listagem" width="100%" bgcolor="#FFFFFF" id="lista_supervisao">
                            <thead>
                            <tr>
                                <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Descrição</b></td>
                                <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Valor (R$)</b></td>
                                <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) Sobre a Obra</b></td>
                                <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Quantidade</b></td>
                                <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Unidade de Medida</b></td>
                                <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Data de Início</b></td>
                                <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Data de Término</b></td>
                                <td colspan="3" rowspan="1" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title" ><b>Última Supervisão</b></td>
                                <td colspan="5" rowspan="1" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title" ><b>Supervisão Atual</b></td>
                            </tr>
                            <tr>
                                <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado</b></td>
                                <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado sobre a Obra</b></td>
                                <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Quantidade já Executada</b></td>

                                <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Quantidade já Executada</b></td>
                                <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) Supervisão</b></td>
                                <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Valor Executado<br></b></td>
                                <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado sobre a Obra após Supervisão</b></td>
                            </tr>
                            </thead>
            <?php
            $total['vlrProjeto'] = 0;
            $total['vlrObra'] = 0;
            $total['vlrExecSobreObra'] = 0;
            $total['vlrExecSobreObraUlt'] = 0;
            $total['percExecSobreObraUlt'] = 0;
            foreach ($dadoEtapaF as $etapa) {
                $total['vlrProjeto'] = 0;

                $total['vlrObra'] += ($etapa['icovlritem'] * $etapa['itcquantidade']);

                if ($etapa['ocrvalorexecucao'] > 0) {
                    $total['percObra'] = ($total['vlrObra'] / $etapa['ocrvalorexecucao']) * 100;
                } else {
                    $total['percObra'] = 0;
                }
                $total['vlrExecSobreObra'] += $etapa['spivlrfinanceiroinfsupervisor'];

                if ($etapa['ocrvalorexecucao'] > 0) {
                    $total['percExecSobreObra'] = ($total['vlrExecSobreObra'] / $etapa['ocrvalorexecucao']) * 100;
                } else {
                    $total['percExecSobreObra'] = 0;
                }

                $total['vlrExecSobreObraUlt'] += $etapa['spivlrfinanceiroanterior'];

                if ($etapa['ocrvalorexecucao'] > 0) {
                    $total['percExecSobreObraUlt'] = ($total['vlrExecSobreObraUlt'] / $etapa['ocrvalorexecucao']) * 100;
                } else {
                    $total['percExecSobreObraUlt'] = 0;
                }
                // $total['percExecSobreObraUlt'] += $edificacao['supvlritemsobreobraexecanterior'];


                $etapa['icodtinicioitem'] = formata_data($etapa['icodtinicioitem']);
                $etapa['icodterminoitem'] = formata_data($etapa['icodterminoitem']);

                if ($etapa['ocrvalorexecucao'] > 0) {
                    // $etapa['spipercsobreobrainfsupervisor'] = ($etapa['spivlrfinanceiroinfsupervisor'] / $etapa['ocrvalorexecucao']) * 100;
                } else {
                    $etapa['spipercsobreobrainfsupervisor'] = 0;
                }

                // Busca Filhos da Etapa
                $dadoDetalhe = $supervisaoItem->getItensByDetalhamento($obrid, $etapa['icoid'], ($supid ? $supid : $ultimoSupid), array("di.ditidpai IS NULL"));
                // $habil = (count($dadoDetalhe) ? 'N' : 'S');
                ?>
                                                <tbody>
                                                <tr bgcolor="#FDF8E7" id="<?php echo "tr_item_etapa_" . $obrid . "_" . $etapa['icoid'] ?>">
                                                    <td align="left">
                                                        <input type="hidden" name="icoid[<?php echo $obrid ?>][]" value="<?php echo $etapa['icoid'] ?>">
                                                        <input type="hidden" name="ico_spiid[<?php echo $obrid ?>][]" value="<?php echo $etapa['spiid'] ?>">
                                                        &nbsp;&nbsp;&nbsp;<img src="/imagens/seta_filho.gif" border="0"><?php echo $etapa['itcdesc'] ?>
                                                    </td>
                                                    <td align="right" style="color:#336EFF">
                <?php echo number_format($etapa['icovlritem'], 2, ',', '.') ?>
                                                    </td>
                                                    <td align="right" style="color:#336EFF">
                <?php echo number_format($etapa['icopercsobreobra'], 2, ',', '.') ?>
                                                    </td>
                                                    <td align="right" style="color:#336EFF">
                <?php echo $etapa['itcquantidade'] ?>
                                                    </td>
                                                    <td align="center">
                <?php echo $etapa['umdeesc'] ?>
                                                    </td>
                                                    <td align="center">
                <?php echo $etapa['icodtinicioitem'] ?>
                                                    </td>
                                                    <td align="center">
                <?php echo $etapa['icodterminoitem'] ?>
                                                    </td>
                                                    <td align="center">
                                                        <input type="hidden" name="ico_spivlrfinanceiroanterior[]" value="<?php echo number_format($etapa['spivlrfinanceiroanterior'], 2, ',', '.') ?>">
                <?php echo campo_texto('ico_spivlritemexecanterior[]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($etapa['spivlritemexecanterior'], 2, ',', '.')); ?>
                                                    </td>
                                                    <td align="center">
                <?php echo campo_texto('ico_spivlritemsobreobraexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($etapa['spivlritemsobreobraexecanterior'], 2, ',', '.')); ?>
                                                    </td>
                                                    <td align="center">
                <?php echo $etapa['spitotitemsupervisor'] ?>
                                                    </td>
                                                    <td align="center" class="ico_spitotitemsupervisor">
                <?php echo campo_texto('ico_spitotitemsupervisor[' . $obrid . '][]', 'N', $somenteLeitura, '', 7, 6, '', '', 'right', '', 0, '', '', number_format($etapa['spitotitemsupervisor'], 2, ',', '.')); ?>
                                                    </td>
                                                    <td align="center">
                <?php echo campo_texto('ico_spivlrinfsupervisor[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($etapa['spivlritemexecanterior'], 2, ',', '.')); ?>
                                                    </td>
                                                    <td align="center">
                <?php echo campo_texto('ico_spivlrfinanceiroinfsupervisor[' . $obrid . '][]', 'N', 'N', '', 11, 15, '', '', 'right', '', 0, '', '', number_format($etapa['spivlrfinanceiroinfsupervisor'], 2, ',', '.')); ?>
                                                    </td>
                                                    <td align="right" class="totpercentitem" style="color:#336EFF">

                                                        <input type="hidden" name="ico_spipercsobreobrainfsupervisor[<?php echo $obrid ?>][]" value="<?php echo number_format($etapa['spivlritemsobreobraexecanterior'], 2, ',', '.') ?>">
                                                                    <span>
                <?php echo number_format($etapa['spivlritemsobreobraexecanterior'], 2, ',', '.') ?>
                                                                    </span>
                                                    </td>

                                                </tr>
                                                </tbody>
                <?php
            }
            ?>
                                        <tbody>
                                        <tr id="tr_total_supervisao_d" bgcolor="#E3E3E3">
                                            <td align="right">
                                                <b>Total</b>
                                                <input type="hidden" name="vlrProjeto" id="vlrProjeto" value="<?php echo $total['vlrProjeto'] ?>">
                                            </td>
                                            <td align="right" style="color:#336EFF">
            <?php echo number_format($total['vlrObra'], 2, ',', '.') ?>
                                            </td>
                                            <td align="right" style="color:#336EFF">---
            <?php echo number_format($total['percObra'], 2, ',', '.') ?>
                                            </td>
                                            <td align="center">
                                                &nbsp;
                                            </td>
                                            <td align="center">
                                                &nbsp;
                                            </td>
                                            <td align="center">
                                                &nbsp;
                                            </td>
                                            <td align="center">
                                                &nbsp;
                                            </td>
                                            <td align="center">
                                                &nbsp;
                                            </td>
                                            <td align="center">
            <?php echo campo_texto('total_spivlritemsobreobraexecanterior', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($total['percExecSobreObraUlt'], 2, ',', '.')); ?>
                                            </td>
                                            <td align="right">
                                                &nbsp;
            <?php // echo campo_texto( 'total_spivlritemexecanterior', 'N', 'N', '', 7, 6, '', '', '', '', 0, '', '', '0,00');  ?>
                                            </td>
                                            <td align="right">
            <?php // echo campo_texto( 'total_spivlrinfsupervisor', 'N', 'N', '', 7, 6, '', '', '', '', 0, '', '', '0,00');  ?>
                                            </td>
                                            <td align="right">
                                                &nbsp;
                                            </td>
                                            <td align="right">
                                                &nbsp;
                                            </td>
                                            <td align="right" style="color:#336EFF" id="td_exec_obra_d">
            <?php //echo number_format($total['percExecSobreObra'], 2, ',', '.')  ?>
                                            </td>
                                        </tr>
                                        </tbody>
                                    </table>
                                </td>
                                </tr>
                <?php endif; ?>
            <?php
        }

        echo "</table>";
        die;
                case 'questionario':
                                        ?>
                        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%" >
                                        <tr>
                                            <td colspan="2">
                                                <div style="float:right">
                                                    <img title="Imprimir questionário" onclick="imprimirQuestionario()" src="../imagens/print.png" class="link">
                                                </div>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td colspan="2">
                                                
                                                <?php
                                              
                                             
                                               
                                                if($os->sosterreno == 't'){
                                                    $qstescopo = "QSTER";
                                                    //$habilitadoPerfil = true;
                                                    //$habilitado = true;
                                                    //$somenteLeitura = 'S';
                                                }
                                                
                                                $questao = new Questao();

                                                $arFiltro = array(
                                                    "qstescopo" => $qstescopo,
                                                    "orgid" => $_SESSION['obras2']['orgid'],
                                                    "sueid" => ($sueid ? $sueid : 0)
                                                );

                                                $arDados = $questao->pegaTodaEstrutura($arFiltro);

                                                //ver($arDados);

                                                $etqidUlt = null;
                                                $itcidUlt = null;
                                                $qstidUlt = null;

                                                $subQuestao = new SubQuestao();
                                                foreach ($arDados as $k => $dados) {

                                                    $arDadosSubQuestao = $subQuestao->pegaSubQuestaoPorQstid($dados['qstid']);

                                                    $qstctrlobs = json_decode($dados['qstctrlobs']);
                                                    switch ($dados['qtsresposta']) {
                                                        case 't':
                                                            $obsClass = ($qstctrlobs->S ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');
                                                            break;
                                                        case 'f':
                                                            $obsClass = ($qstctrlobs->N ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');
                                                            break;
                                                        case 'n':
                                                            $obsClass = ($qstctrlobs->NA ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');
                                                            break;
                                                        default:
                                                            $obsClass = 'divQuestaoNivel3_none';
                                                            break;
                                                    }

                                                    $qstctrlimg = json_decode($dados['qstctrlimagem']);
                                                    switch ($dados['qtsresposta']) {
                                                        case 't':
                                                            $imgClass = ($qstctrlimg->S ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');
                                                            break;
                                                        case 'f':
                                                            $imgClass = ($qstctrlimg->N ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');
                                                            break;
                                                        case 'n':
                                                            $imgClass = ($qstctrlimg->NA ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');
                                                            break;
                                                        default:
                                                            $imgClass = 'divQuestaoNivel4_none';
                                                            break;
                                                    }


                                                    // ABRE Questionário
                                                    if ($etqidUlt != $dados['etqid']) {
                                                        if (!is_null($etqidUlt)) {
                                                            $html .= "</fieldset>";
                                                        }
                                                        $etqidUlt = $dados['etqid'];
                                                        $html .= "<fieldset><legend>{$dados['etqdsc']}</legend>";
                                                    }

                                                    // ETAPA
                                                    if ($itcidUlt != $dados['itcid']) {
                                                        $itcidNum++;
                                                        $html .= "<div class='divItemComposicao'>{$itcidNum} - {$dados['itcdsc']}</div>";
                                                        $itcidUlt = $dados['itcid'];
                                                    }

                                                    // DIVISÃO
                                                    if ($dvqidUlt != $dados['dvqid'] && !empty($dados['dvqid'])) {
                                                        $html .= "<div class='divDivisao'>{$dados['dvqnumero']} - {$dados['dvqdsc']}</div>";
                                                        $dvqidUlt = $dados['dvqid'];
                                                    }


                                                    // QUESTÃO
                                                    if ($qstidUlt != $dados['qstid']) {

                                                        // Sub Questão
                                                        $subClass = ($dados['qtsresposta'] == 'f' ? 'divSubQuestao' : 'divSubQuestao_none' );
                                                        $subItemClass = 'divSubItemQuestao';

                                                        $subImgClass = "divSubQuestaoNivel4_none";
                                                        $htmlSubQuestao = "";
                                                        if (count($arDadosSubQuestao) > 0) {
                                                            $htmlSubQuestao .= "<div id=\"div_subquestao_{$dados['qstid']}_{$dadosSubQuestao['sqtid']}\" class=\"{$subClass}\">
                                                                                                                                <div class=\"divSubQuestaoNivel1\">Tipo:</div>";
                                                        }
                                                        foreach ($arDadosSubQuestao as $dadosSubQuestao) {
                                                            $arResultadosDadosSubQuestao = $subQuestao->pegaResultadosSubQuestaoPorSqtidQtsid($dadosSubQuestao['sqtid'], ($dados['qtsid'] ? $dados['qtsid'] : 0));
                                                            $subObsClass = $arResultadosDadosSubQuestao[0]['rsqstatus'] == 'A' ? "divSubQuestaoNivel3" : "divSubQuestaoNivel3_none";
                                                            $checked = $arResultadosDadosSubQuestao[0]['rsqstatus'] == 'A' ? "checked=\"checked\"" : '';
                                                            $restricao = '';
                                                            if ($dadosSubQuestao['mrqid'] != '') {
                                                                $restricao = "<input type=\"hidden\" name=\"mrqid[{$dados['qstid']}][{$dadosSubQuestao['sqtid']}]\" value=\"{$dadosSubQuestao['mrqid']}\">";
                                                            }

                                                            $htmlSubQuestao .= "<div id=\"div_subquestao_item_{$dadosSubQuestao['sqtid']}\" class=\"{$subItemClass}\">
                                                                                                                                <div class=\"divSubQuestaoNivel2\">
                                                                                                                                        <input type=\"checkbox\" $checked
                                                                                                                                                   value=\"{$dadosSubQuestao['sqtid']}\"
                                                                                                                                                   name=\"sqtid[{$dados['qstid']}][]\"
                                                                                                                                                   id=\"sqtid_{$dadosSubQuestao['sqtid']}\"
                                                                                                                                                   " . ($habilitado ? '' : 'disabled="disabled"') . "
                                                                                                                                                   onclick=\"ctrlImgObs(this, {$dadosSubQuestao['sqtid']});\">
                                                                                                                                        <label for=\"sqtid_{$dadosSubQuestao['sqtid']}\">
                                                                                                                                        {$dadosSubQuestao['sqtdsc']}
                                                                                                                                        </label>
                                                                                                                                </div>
                                                                                                                                        <div class='" . $subObsClass . "' id='div_obs_sub_{$dadosSubQuestao['sqtid']}'>
                                                                                                                                                <div class='divSubQuestaoNivel3_1'>Observação:</div>
                                                                                                                                                $restricao
                                                                                                                                                <input type=\"hidden\" name=\"rsqid[{$dados['qstid']}][{$dadosSubQuestao['sqtid']}]\" value=\"{$arResultadosDadosSubQuestao[0]['rsqid']}\">
                                                                                                                                                " . campo_textarea('rsqobs_' . $dados['qstid'] . '_' . $dadosSubQuestao['sqtid'], 'S', $somenteLeitura, '', 100, 4, 1000, '', '', '', '', '', stripslashes($arResultadosDadosSubQuestao[0]['rsqobs']), array('id' => "qtsobs_sub_{$dadosSubQuestao['sqtid']}")) . "
                                                                                                                                        </div>
                                                                                                                                </div>";
                                                            $htmlSubQuestaoTipo = '';
                                                            // Quando houver subquestão e a resposta for "não" virá fechada a OBSERVAÇÃO
                                                            if ($dados['qtsresposta'] == 'f') {
                                                                $obsClass = 'divQuestaoNivel3_none';
                                                                $dados['qtsobs'] = '';
                                                            }
                                                        }
                                                        if (count($arDadosSubQuestao) > 0) {
                                                            $htmlSubQuestao .= "</div>";
                                                        }

                                                        $html .= "<div class='divQuestao'>
                                                                                                        <div class='divQuestaoNivel1'>{$dados['qstnumero']} - {$dados['qstdsc']}</div>
                                                                                                        <div class='divQuestaoNivel2'>
                                                                                                                <input type='hidden' name='qtsid_{$dados['qstid']}' id='qtsid_{$dados['qstid']}' value='{$dados['qtsid']}'>
                                                                                                                <input
                                                                                                                        name='qstid[{$dados['qstid']}]'
                                                                                                                        id='qstid_t_{$dados['qstid']}' value='t'
                                                                                                                        type='radio'
                                                                                                                        " . ($habilitado ? '' : 'disabled="disabled"') . "
                                                                                                                        onclick='ctrlObs({$dados['qstid']}, " . ($qstctrlobs->S ? 'true' : 'false' ) . "); ctrlSubPergunta({$dados['qstid']}, false);' " .
                                                                ($dados['qtsresposta'] == 't' ? "checked='checked'" : "") . ">
                                                                                                                <label for='qstid_t_{$dados['qstid']}'>Sim</label>
                                                                                                                &nbsp;&nbsp;
                                                                                                                <input
                                                                                                                        name='qstid[{$dados['qstid']}]'
                                                                                                                        id='qstid_f_{$dados['qstid']}'
                                                                                                                        value='f'
                                                                                                                        type='radio'
                                                                                                                        " . ($habilitado ? '' : 'disabled="disabled"') . "
                                                                                                                        onclick='ctrlObs({$dados['qstid']}, " . ($qstctrlobs->N ? 'true' : 'false' ) . "); ctrlSubPergunta({$dados['qstid']}, true);' " .
                                                                ($dados['qtsresposta'] == 'f' ? "checked='checked'" : "") . ">
                                                                                                                <label for='qstid_f_{$dados['qstid']}'>Não</label>
                                                                                                                &nbsp;&nbsp;
                                                                                                                <input
                                                                                                                        name='qstid[{$dados['qstid']}]'
                                                                                                                        id='qstid_n_{$dados['qstid']}'
                                                                                                                        value='n'
                                                                                                                        type='radio'
                                                                                                                        " . ($habilitado ? '' : 'disabled="disabled"') . "
                                                                                                                        onclick='ctrlObs({$dados['qstid']}, " . ($qstctrlobs->NA ? 'true' : 'false' ) . "); ctrlSubPergunta({$dados['qstid']}, false);' " .
                                                                ($dados['qtsresposta'] == 'n' ? "checked='checked'" : "") . ">
                                                                                                                <label for='qstid_n_{$dados['qstid']}'>Não se aplica</label>
                                                                                                        </div>
                                                                                                        " . $htmlSubQuestao . "
                                                                                                        <div class='" . $obsClass . "' id='div_obs_{$dados['qstid']}'>
                                                                                                                <div class='divQuestaoNivel3_1'>Observação:</div>
                                                                                                                " . campo_textarea('qtsobs_' . $dados['qstid'], 'S', $somenteLeitura, '', 100, 4, 1000, '', '', '', '', '', stripslashes($dados['qtsobs']), array('id' => "qtsobs_{$dados['qstid']}")) . "
                                                                                                        </div>";

                                                        $html .= "</div>";
                                                        $qstidUlt = $dados['qstid'];
                                                    }

                                                    echo $html;
                                                    $html = null;
                                                }
                                                ?>
                                            </td>
                                        </tr>
                                    </table>
                              
                                
                                
                    <?php
                    die;
                case 'fotos':
                    if ($habilitado){
                ?>
                    <script>
                        jQuery(document).ready(function() {

                                jQuery( ".originais" ).draggable({
                                        connectToSortable: ".div_fotos2",
                                        helper: "clone",
                                        revert: "invalid",
                                        stop: function( event, ui ) {
                                                var arrId = jQuery(this).attr('id').split('_');
                                                var Id = arrId[1];
                                                if( arrId[2] != '' ){
                                                        Id = arrId[2];
                                                }
                                                jQuery('#fotos_galeria').find("#s_" + arrId[1]).attr("class","draggable originais f_selected");
                                        }
                                }).sortable({
                                        cancel: ".nodraggable"
                                });

                                jQuery( ".div_fotos2" ).droppable({
                                        drop: function( event, ui ) {
                                                var tableta = jQuery(this).attr('id').split('_');
                                                var campo 	= tableta[1];
                                                tableta		= tableta[0]
                                                var id 		= jQuery(this).attr('name');
                                                var arqid  	= jQuery(this).children('.originais').attr('name');
                                                //console.log(id + ' - ' + arqid);
                                                if( id && arqid ){
                                                        var url = window.location + "&ajax=gravarFoto&tabela="+tableta+"&campo="+campo+"&id="+id+"&arqid="+arqid;
                                                        jQuery.ajax({
                                                                url: url,
                                                                success: function(data) {
                                                          }
                                                        });
                                                }
                                                jQuery(this).children().removeClass("f_selected");
                                                jQuery(this).children().removeClass("originais");
                                                jQuery(this).children().removeClass("draggable");
                                                jQuery(this).children().addClass("nodraggable");
                                                jQuery(this).children().find('.img_class').addClass("img_class2");
                                                jQuery(this).children().find('.img_class').removeClass("img_class");
                                                jQuery(this).find('.fechar').show();
                                        }
                                }).sortable({
                                        revert: false,
                                        placeholder: "draggable_space",
                                        cancel: ".nodraggable"
                                });
                                jQuery( "ul, li" ).disableSelection();
                        });

                    </script>
                <?php
        }
            ?>
                                            <table class="tabela" bgcolor="white" cellspacing="1" cellpadding="3" align="center" border="0" style="width:100%">
                                                    <tbody>
                                                            <tr bgcolor="#DCDCDC">
                                                                    <td width="100%" align="center" colspan="2">
                                                                            <label class="TituloTela" style="color:#000000;">Cadastro de Fotos da Supervisão</label>
                                                                    </td>
                                                            </tr>
                                                            <tr>
                                                                    <td bgcolor="#e9e9e9" style="text-align: center; width: 180px;">
            <?php if ($_SESSION['obras2']['sueid'] && $habilitado && $stsSup): ?>
                                                                                <a onclick="incluirFotos('<?php echo $_SESSION['obras2']['sueid'] ?>');" title="Enviar arquivos">
                                                                                        <img src="../imagens/gadget_arvore.png" title="Enviar arquivos" border="0" align="absmiddle"/><br/>
                                                                                        <b>Enviar arquivos</b>
                                                                                </a>
            <?php elseif ($habilitado): ?>
                                                                                Erro! O registro da supervisão da empresa não existe no banco de dados.
            <?php endif; ?>
                                                                    </td>
                                                                    <td bgcolor="#e9e9e9" align="center" style="width:100%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')">
                                                                            <p>
                                                                                    Todas as fotos devem estar no formato JPG<br>
                                                                                    Possuir marcação GPS no arquivo<br>
                                                                                    Terem menos de 4 MB.
                                                                            </p>
                                                                    </td>
                                                            </tr>
                                                    </tbody>
                                            </table>
                                            <table class="tabela" bgcolor="white" cellspacing="1" cellpadding="3" align="center" border="0" style="width:100%">
                                                    <tbody>
                                                            <tr>
                                                                    <td class="container-perguntas" width="60%">
                                                                        <!-- Alteração -->
                                                                        <div class="container-perguntas" style="height: 528px; overflow: scroll;">
            <?php
            $_SESSION['downloadfiles']['pasta'] = array("origem" => "obras2", "destino" => "obras2");
            //							$_SESSION['imgparams'] = array("filtro" => " 1=1 ", "tabela" => "obras2.arquivoquestaosupervisao");

            $questao = new Questao();
            $arquivoQuestaoSupervisao = new ArquivoQuestaoSupervisao();
if($os->sosterreno == 't'){
   $qstescopo = "QSTER";
}
            $arFiltro = array("qstescopo" => $qstescopo,
                "orgid" => $_SESSION['obras2']['orgid'],
                "sueid" => ($sueid ? $sueid : 0));
            $arDados = $questao->pegaTodaEstrutura($arFiltro);

            $etqidUlt = null;
            $itcidUlt = null;
            $qstidUlt = null;
            $temImg = false;

            $subQuestao = new SubQuestao();
            $arquivoRespostaSubQuestao = new ArquivoRespostaSubQuestao();

            foreach ($arDados as $k => $dados) {

                $arDadosSubQuestao = $subQuestao->pegaSubQuestaoPorQstid($dados['qstid']);

                $qstctrlobs = json_decode($dados['qstctrlobs']);
                switch ($dados['qtsresposta']) {
                    case 't':
                        $obsClass = ($qstctrlobs->S ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');
                        break;
                    case 'f':
                        $obsClass = ($qstctrlobs->N ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');
                        break;
                    case 'n':
                        $obsClass = ($qstctrlobs->NA ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');
                        break;
                    default:
                        $obsClass = 'divQuestaoNivel3_none';
                        break;
                }

                $qstctrlimg = json_decode($dados['qstctrlimagem']);
                switch ($dados['qtsresposta']) {
                    case 't':
                        $imgClass = ($qstctrlimg->S ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');
                        $temImg = $qstctrlimg->S;
                        break;
                    case 'f':
                        $imgClass = ($qstctrlimg->N ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');
                        $temImg = $qstctrlimg->N;
                        break;
                    case 'n':
                        $imgClass = ($qstctrlimg->NA ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');
                        $temImg = $qstctrlimg->NA;
                        break;
                    default:
                        $imgClass = 'divQuestaoNivel4_none';
                        break;
                }


                // ABRE Questionário
                if ($etqidUlt != $dados['etqid']) {
                    if (!is_null($etqidUlt)) {
                        $htmlTemp .= "</fieldset>";
                    }
                    $etqidUlt = $dados['etqid'];
                    $htmlTemp .= "<fieldset> <legend>{$dados['etqdsc']}</legend> <div id=\"div_questoes\" style=\"height:500px;overflow:auto;\">";
                }

                // ETAPA
                if ($itcidUlt != $dados['itcid']) {
                    $itcidNum++;
                    $htmlTemp .= "<div class='divItemComposicao'>{$itcidNum} - {$dados['itcdsc']}</div>";
                    $itcidUlt = $dados['itcid'];
                }

                // DIVISÃO
                if ($dvqidUlt != $dados['dvqid'] && !empty($dados['dvqid'])) {
                    $htmlTemp .= "<div class='divDivisao'>{$dados['dvqnumero']} - {$dados['dvqdsc']}</div>";
                    $dvqidUlt = $dados['dvqid'];
                }

                // QUESTÃO
                if ($qstidUlt != $dados['qstid']) {

                    // Sub Questão
                    $subClass = ($dados['qtsresposta'] == 'f' ? 'divSubQuestao' : 'divSubQuestao_none' );
                    $subItemClass = 'divSubItemQuestao';

                    $subImgClass = "divSubQuestaoNivel4";
                    $htmlSubQuestao = "";

                    if (is_array($arDadosSubQuestao)) {
                        if (count($arDadosSubQuestao) > 0) {
                            $htmlSubQuestao .= "<div id=\"div_subquestao_{$dados['qstid']}_{$dadosSubQuestao['sqtid']}\" class=\"{$subClass}\">
                                                                                                                                        <div class=\"divSubQuestaoNivel1\"></div>";
                            $temImgSub = false;

                            foreach ($arDadosSubQuestao as $dadosSubQuestao) {

                                $arResultadosDadosSubQuestao = $subQuestao->pegaResultadosSubQuestaoPorSqtidQtsid($dadosSubQuestao['sqtid'], ($dados['qtsid'] ? $dados['qtsid'] : 0));

                                $temImgSub = false;
                                $temImgSub = ($temImgSub || $dadosSubQuestao['sqtimg'] == 't') && $arResultadosDadosSubQuestao[0]['rsqstatus'] == 'A';

                                if ($arResultadosDadosSubQuestao[0]['rsqstatus'] == 'A') {
                                    $htmlSubQuestao .= "<div id=\"div_subquestao_item_{$dadosSubQuestao['sqtid']}\" >
                    <div class='$subImgClass' id='div_img_sub_{$dadosSubQuestao['sqtid']}'>
                            <fieldset class=\"field_fotos\" id=\"field_img_sub_{$dadosSubQuestao['sqtid']}\">
                                    <legend>Fotos da Subquestão - {$dadosSubQuestao['sqtdsc']} <div class='subquestaoobs'><span style='font-weight:bold'>Observação: </span>{$arResultadosDadosSubQuestao[0]['rsqobs']}</div></legend>
                                    <ul id=\"arquivorespostasubquestao_rsqid\" name=\"{$arResultadosDadosSubQuestao[0]['rsqid']}\" class=\"div_fotos div_fotos2\">";
                                    $arrArquivosSubQuestão = $arquivoRespostaSubQuestao->listaPorRespQuestao($arResultadosDadosSubQuestao[0]['rsqid']);
                                    if (is_array($arrArquivosSubQuestão)) {
                                        foreach ($arrArquivosSubQuestão as $arquivoSub) {
                                            $htmlSubQuestao .= '<li class="ui-draggable ui-sortable nodraggable" name="' . $arquivoSub['arqid'] . '" id="s_' . $arquivoSub['arqid'] . '" unselectable="on" style="-moz-user-select: none; display: list-item;">
                                                                                                                                                    <img onclick="removerFotoGaleria(this,' . $arquivoSub['arsid'] . ')" style="' . $display . '" class="fechar" title="Remover Foto" src="../imagens/fechar.jpeg">
                                                                                                                                                    <img width="91" height="68.25" src="../slideshow/slideshow/verimagem.php?arqid=' . $arquivoSub['arqid'] . '&amp;newwidth=100&amp;newheight=85"
                                                                                                                                                             ondblclick="abrirGaleria(\'' . $arquivoSub['arqid'] . '\',\'0\',this)" class="img_class2">
                                                                                                                                            </li>';
                                        }
                                    }
                                    $htmlSubQuestao .= "</ul>
                                                                                                                                        </fieldset>
                                                                                                                                        </div>
                                                                                                                                    </div>";
                                    $htmlSubQuestaoTipo = '';
                                }
                            }
                            $htmlSubQuestao .= "</div>";
                        }
                    }

                    if (($temImg || $temImgSub) && $dados['qtsresposta'] != '') {
                        $htmlTemp .= "<div class='divQuestao'>
                                                                                                                        <fieldset class=\"field_fotos\" id=\"field_img_{$dadosSubQuestao['sqtid']}\">
                                                                                                                                <legend>Fotos da Questão - {$dados['qstnumero']} - {$dados['qstdsc']} Resp.: " . ($dados['qtsresposta'] == 't' ? "Sim" : ($dados['qtsresposta'] == 'f' ? "Não" : "Não se aplica")) . " </legend>
                                                                                                                                <ul id=\"arquivoquestaosupervisao_qtsid\" name=\"{$dados['qtsid']}\" class=\"div_fotos div_fotos2\">";
                        $arrArquivosQuestão = $arquivoQuestaoSupervisao->listaPorRespQuestao($dados['qtsid']);
                        if (is_array($arrArquivosQuestão)) {
                            foreach ($arrArquivosQuestão as $arquivo) {
                                $htmlTemp .= '			<li class="ui-draggable ui-sortable nodraggable" name="' . $arquivo['arqid'] . '" id="s_' . $arquivo['arqid'] . '" unselectable="on" style="-moz-user-select: none; display: list-item;">
                                                                                                                                                <img onclick="removerFotoGaleria(this,' . $arquivo['pk_questao_foto'] . ')" style="' . $display . '" class="fechar" title="Remover Foto" src="../imagens/fechar.jpeg">
                                                                                                                                                <img width="91" height="68.25" src="../slideshow/slideshow/verimagem.php?arqid=' . $arquivo['arqid'] . '&amp;newwidth=100&amp;newheight=85"
                                                                                                                                                         ondblclick="abrirGaleria(\'' . $arquivo['arqid'] . '\',\'0\',this)" class="img_class2">
                                                                                                                                        </li>';
                            }
                        }
                        $htmlTemp .= "		</ul>
                                                                                                                  </fieldset>
                                                                                                                " . $htmlSubQuestao . "
                                                                                                                <div class='" . $obsClass . "' id='div_obs_{$dados['qstid']}'>
                                                                                                                </div>";
                        $htmlTemp .= "</div>";
                    } else {
                        $htmlTemp = '';
                    }
                    $html = $html . $htmlTemp;
                    $htmlTemp = null;
                    $qstidUlt = $dados['qstid'];
                }

                if ($html != '') {
                    echo $html;
                    $html = null;
                    $escreveu = true;
                } else {
                    $escreveu = $escreveu || false;
                }
            }
            if (!$escreveu) {
                echo '<label style="color:red;">Essa vistoria não possui questões com anexo.</label>';
            }
            ?>
                                                                            </div>
                                                                    </div>
                                                            </td>
                                                            <td rowspan="2">
                                                                            <fieldset class="field_fotos" style="width:92%;height:500px;">
                                                                                    <legend>Fotos da Galeria</legend>
                                                                                    <ul id="fotos_galeria" class="div_fotos">
            <?php
            if (!empty($sueid)) {
                $sql = "SELECT arqid FROM obras2.arquivosupervisao WHERE aqsstatus = 'A' AND sueid = $sueid ORDER BY arqid";
                $arrFotosGaleria = $db->carregar($sql);
            } else {
                $arrFotosGaleria = array();
            }
            ?>
            <?php if (is_array($arrFotosGaleria)): ?>
                <?php $n = 1 ?>
                <?php foreach ($arrFotosGaleria as $foto): ?>
                    <?php $pagina = floor($n / 16); ?>

                    <?php
                    $sql = "SELECT arqtipo, arqid  FROM public.arquivo LIMIT 10";
                    $dados = $db->pegaLinha($sql);

                    $imgend = APPRAIZ . 'arquivos/' . (($_REQUEST["_sisarquivo"]) ? $_REQUEST["_sisarquivo"] : $_SESSION["sisarquivo"]) . '/' . floor($foto['arqid'] / 1000) . '/' . $foto['arqid'];

                    if (is_file($imgend)) {
                        $img_max_dimX = 100;
                        $img_max_dimY = 85;

                        $imginfo = getimagesize($imgend);

                        $width = $imginfo[0];
                        $height = $imginfo[1];

                        if (($width > $img_max_dimX) or ( $height > $img_max_dimY)) {
                            if ($width > $height) {
                                $w = $width * 0.9;
                                while ($w > $img_max_dimX) {
                                    $w = $w * 0.9;
                                }
                                $w = round($w);
                                $h = ($w * $height) / $width;
                            } else {
                                $h = $height * 0.9;
                                while ($h > $img_max_dimY) {
                                    $h = $h * 0.9;
                                }
                                $h = round($h);
                                $w = ($h * $width) / $height;
                            }
                        } else {
                            $w = $width;
                            $h = $height;
                        }

                        $tamanho = " width=\"$w\" height=\"$h\" ";
                    } else {
                        $tamanho = "";
                    }

                    $testaSelecionado = testaSelecionado($foto['arqid'], $sueid);
                    ?>
                                                                                                                    <li id="s_<?php echo $foto['arqid'] ?>" name="<?php echo $foto['arqid'] ?>" class="draggable <?php echo $cssClass; ?> <?php echo $testaSelecionado ?>">
                    <?php if ($stsSup) { ?>
                                                                                                                                <img src="../imagens/fechar.jpeg" title="Remover Foto" class="fechar" style="display:none" onclick="removerFotoGaleria(this,0)" />
                    <?php } else { ?>
                                                                                                                                <img src="../imagens/fechar_01.jpeg" title="Remover Foto" class="fechar" style="display:none"/>
                    <?php } ?>
                                                                                                                            <img <?php echo $tamanho; ?> class="img_class" ondblclick="abrirGaleria('<?php echo $foto['arqid'] ?>','<?php echo $pagina ?>',this)" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqid'] ?>&newwidth=100&newheight=85" />
                                                                                                                    </li>
                    <?php $n++ ?>
                <?php endforeach; ?>
            <?php endif; ?>
                                                                                    </ul>
                                                                            </fieldset>
                                                                    </td>
                                                        </tr>
                                                    </table>
            <?php
            die;
            break;
                case 'tramitacao':

                if (empty($_SESSION['obras2']['sueid'])) { ?>
                    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%" >
                       <tr>
                           <td>
                           </td>
                       </tr>
                   </table>
                <?php
                } else {

                    $valorsp = percentualSupEmpresa($empid);
                    $diff   = $valorsp['percental']['empresa'] - $valorsp['percental']['unidade'];
                    //ver($diff);

                    //pega as restrições
                    $collection = pegaColecaoRestricao($_SESSION['obras2']['sueid']);
                    //ver($collection);
                ?>
                </table>
                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%">
                    <tr>
                        <td colspan="2">
                            <div>
                                <img title="Imprimir questionário respondido" onclick="imprimirQuestionarioRespondido()" src="../imagens/print.png" class="link"> Imprimir questionário respondido
                            </div>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" style="width: 300px;">Sinalizar ao FNDE que existe problema muito grave nesta obra? (risco de desabamento, má aplicação de recursos público, etc)</td>
                        <td>
                            <div class="divQuestaoNivel2">
                                <input name="sueproblema" id="sueproblema" class="sueproblema" value="t" type="radio" <?=($sueproblema == 't') ? 'checked="checked"' : ''?> >
                                <label for="qstid_t_2">Sim</label>
                                &nbsp;&nbsp;
                                <input name="sueproblema" id="sueproblema" class="sueproblema" value="f" type="radio" <?=($sueproblema == 'f') ? 'checked="checked"' : ''?>>
                                <label for="qstid_f_2">Não</label> <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                            </div>
                            <div class="divQuestaoNivel sueobsproblema" <?=($sueproblema != 't') ? 'style="display: none"' : ''?>>
                                <b>Observação:<br /><br />
                                <?= campo_textarea('sueobsproblema', 'S', 'S', '', '70', '8', '5000', '', 0, '', '', false, '', ''); ?>
                            </div>
                        </td>
                    </tr>
                    <script type="text/javascript">
                        jQuery(function(){
                            jQuery('[name="sueproblema"]').change(function (e) {
                                if(jQuery(this).val() == 't')
                                    jQuery('.sueobsproblema').show();
                                else
                                    jQuery('.sueobsproblema').hide();
                            })
                        });
                        function enviaFormulario(){
                            if(jQuery('[name="sueproblema"]:checked').val() == 't' && jQuery('[name="sueobsproblema"]').val().trim() == ''){
                                alert('O campo observaçao deve ser preenchido.')
                                return false;
                            }
                            jQuery('#formulario_supervisao_empresa').submit();
                        }
                    </script>
                    <?php if ($diff > '10.0' || $diff < '-10.0') { ?>
                    <tr>
                        <td class="SubTituloDireita">Justificativa da defasagem</td>
                        <td>
                            <?= campo_textarea('suejustificativa', 'S', 'S', '', '70', '8', '5000', '', 0, '', '', false, '', ''); ?>
                        </td>
                    </tr>
                    <?php } ?>
                    <?php  if (count($collection)) : ?>
                    <tr>
                        <td colspan="2" class="SubTituloEsquerda">Descrição da restrição</td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <?php
                            $tmpId = null;
                            foreach ($collection as $k => $linhaQ) {

                                if ($tmpId == $linhaQ['qstid']) {
                                    if ($linhaQ['rstitem'] == 'R') {
                                        echo '<tr><td>&nbsp;&nbsp; - ' . $linhaQ['sqtdsc'] . '</td></tr>';
                                        subQuestao($linhaQ);
                                    }
                                    continue;
                                } else {
                                    if ($k > 0) echo '</table>';
                                }

                                echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:95%">';
                                echo '<tr><td><strong style="font-size:14px;color:#333;">' . $linhaQ['qstdsc'] . '</strong></td></tr>';
                                echo '<tr><td>&nbsp;&nbsp;<span style="font-size:13px;color:#000;">' . pegaRespostaQuestao($linhaQ['qstctrlobs'], $linhaQ['qtsresposta']) . '</span></td></tr>';

                                //se houver subquestao
                                if (!empty($linhaQ['sqtdsc']) && $linhaQ['rstitem'] == 'R') {
                                    echo '<tr><td>&nbsp;&nbsp;<span style="font-size:13px;color:#000;"> - ' . $linhaQ['sqtdsc'] . '</span></td></tr>';
                                }

                                subQuestao($linhaQ);
                                $tmpId = $linhaQ['qstid'];
                            }
                            ?>
                        </td>
                    </tr>
                    <?php endif; ?>
                </table>
                <script type="text/javascript">
                    jQuery(function(){
                        jQuery(".input-check")
                        .click(function(){
                            var rel = jQuery(this).attr("rel");
                            if (jQuery(this).attr("value") == "S") {
                                jQuery("#div-suetiporiscoobs"+rel).show();
                            } else {
                                jQuery("#div-suetiporiscoobs"+rel).hide();
                            }
                        });
                    });
                </script>
                <?php
                }
                die;
            case 'restricao':
                $supervisaoER = new SupervisaoEmpresaRestricao();

                if($stsSup) {
                    $sql = $supervisaoER->retornaQueryCadastramento($obra->obrid, $sueid);
                } else {
                    $sql = $supervisaoER->retornaQueryPosCadastramento($sueid);
                }

                $cabecalho = array("Superar","ID Item", "Item", "Fase","Tipo", "Data da Inclusão", "Descrição", "Providência", "Previsão da Providência", "Criado Por", "Superação", "Situação Atual", "Superado Por", "Ressalva", "Último Tramite", "Data do Último Tramite");
                $db->monta_lista($sql, $cabecalho, 30, 10,'N','center', 'N', '',array('13%','','','','','','','','','','','','','','',''));
                die;
        }
}else{
	$_SESSION['obras2']['abaAjax'] = $_SESSION['obras2']['abaAjax'] ? $_SESSION['obras2']['abaAjax'] : 'dadosSupervisao';
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

if( $_SESSION['obras2']['sueid'] ){
	$db->cria_aba(ID_ABA_CADASTRO_VISTORIA_EMPRESA_EDICAO,$url,$parametros);
}else{
	$db->cria_aba(ID_ABA_CADASTRO_VISTORIA_EMPRESA,$url,$parametros);
}
$obra = new Obras();
$obra->popularDadosObjeto($obra->pegaObraPorEmpid($empid));

echo cabecalhoObra($obra->obrid, 'simples');

$mi = '';

if($obra->tpoid == 104 || $obra->tpoid == 105){
    echo '<div style="color: red; position: absolute; top: 290px; right: 60px; float: right; z-index:1;">
            <img border="0" title="Obra de Metodologia Inovadora." src="../imagens/carimbo-mi.png">
          </div>';
}

monta_titulo( 'Cadastro de Supervisão', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.' );
?>
<!--<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>-->
                <!--<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>-->
<!--<script language="javascript" type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>-->
<!--<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>-->
<!--<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>-->
<!--<script src="/includes/prototype.js"></script>-->
<!--<script src="/includes/entidades.js"></script>-->
<!--<script type="text/javascript" src="../includes/funcoes.js"></script>-->

          <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
          <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
          <script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
          <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
          <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
          <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
          <script language="JavaScript" src="../includes/funcoes.js"></script>
          <script>jQuery.noConflict();</script>
<script src="/includes/prototype.js"></script>
<script src="/includes/entidades.js"></script>



<style>
.subquestaoobs{padding-bottom: 4px;}
.div_fotos { list-style-type: none; margin: 0; padding: 0;padding-top:3px}
.div_fotos li { font-size: 1.2em; height: 110px; height:90px;padding: 1px}
html>body .div_fotos li { height: 80px; line-height: 1.2em; }
.field_fotos{/*padding:10px;*/width:90%}
.div_fotos{height:499px;overflow:auto;}
.div_fotos2{height:90px;overflow:auto;}
.draggable{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;cursor:move;text-align:center;background-color:#FFFFFF}
.nodraggable{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;text-align:center;background-color:#FFFFFF}
.draggable_space{line-height: 1.2em;width:110px;height:90px;margin:3px;float:left;cursor:pointer;background-color:#CCCCCC}
.f_selected{border: solid 1px lightgreen;background-color: lightgreen;}
.fechar{position:relative;margin-left:105px;top:-8px;cursor:pointer}
.img_foto{z-index:2;}
.img_class{margin-top:5px;}
.img_class2{margin-top:-10px;}

.div_abas_ajax{width:99%;background-color: #FFFFFF;overflow:fixed;}

.divItemComposicao{
	width:100%;
	font-weight:bold;
}

.divDivisao{
	width:100%;
	margin-left:5px;
	font-weight:bold;
	padding:5px;
}

.divQuestao{
	width:100%;
	margin-left:10px;
	padding:5px;
}
.divQuestaoNivel1{
	width:100%;
}
.divQuestaoNivel2{
	width:100%;
}
.divQuestaoNivel3{
	margin-top:3px;
}
.divQuestaoNivel4{
	margin-top:3px;
}
.divQuestaoNivel4_none{
	margin-top:3px;
	display:none;
}
.divQuestaoNivel4_fieldset{
	width:98%
}
.divQuestaoNivel4_1{
	margin-top:3px;
}
.divQuestaoNivel4_2{
	margin-top:3px;
}
.divQuestaoNivel4_3{
	margin-top:3px;
	padding:2px 2px 2px 2px;
	border: 1px dashed #ccc;
	height: 70px;
}
.divQuestaoNivel3{
	margin-top:3px;
}
.divQuestaoNivel3_none{
	display:none;
	margin-top:3px;
}
.divQuestaoNivel3_1{
	font-weight:bold;
}

.divSubQuestao{
	padding-left: 20px;
	padding-top: 10px;
	padding-bottom: 5px;
}
.divSubQuestao_none{
	display: none;
}

.divSubItemQuestao{
	padding: 5px, 5px, 5px, 5px;
}
.divSubItemQuestao_none{
	display: none;
}

.divSubQuestaoNivel1{
	font-weight: bold;
}
.divSubQuestaoNivel1_none{}

.divSubQuestaoNivel2{
	font-weight: bold;
}
.divSubQuestaoNivel2_none{}

.divSubQuestaoNivel3{
	padding-left: 30px;
}
.divSubQuestaoNivel3_none{
	display: none;
}

.divSubQuestaoNivel4{
	padding-left: 30px;
	width: 90%
}
.divSubQuestaoNivel4_none{
	display: none;
}
</style>
<form id="formulario_supervisao_empresa" name="formulario_supervisao_empresa" method="post" enctype="multipart/form-data" action="">

	<input type="hidden" name="sueid"               id="sueid"         value="<?php echo $sueid ?>" />
	<input type="hidden" name="requisicao"          id="requisicao"    value="salvar" />

	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
		<tr>
			<td colspan="2" style="heigth:600px;">
				<div style="width:100%;height:600px;overflow:auto;">
			 	<?php
			 	include(APPRAIZ."/includes/classes/AbaAjax.class.inc");

			 	$abaAjax = new AbaAjax("abasAjax",false,false,false,false);

		 		$arAba = array(
	 		  			array(	 "descricao" => "Dados da Supervisão",
		 	   				 	 "padrao" =>  ($_SESSION['obras2']['abaAjax'] == 'dadosSupervisao' ? true : false)  ,
		 		   				 "url" => "obras2.php?modulo=principal/cadVistoriaEmpresa&acao={$_GET['acao']}",
		 		   				 "parametro" => "abaAjax=dadosSupervisao"
	 		  			),
		 				array(	 "descricao" => "Local da Obra",
		 						"padrao" => ($_SESSION['obras2']['abaAjax'] == 'localObra' ? true : false) ,
		 						"url" => "obras2.php?modulo=principal/cadVistoriaEmpresa&acao={$_GET['acao']}",
		 						"parametro" => "abaAjax=localObra"
		 				),
		 				array(	 "descricao" => "Cronograma",
		 						"padrao" => ($_SESSION['obras2']['abaAjax'] == 'cronograma' ? true : false) ,
		 						"url" => "obras2.php?modulo=principal/cadVistoriaEmpresa&acao={$_GET['acao']}",
		 						"parametro" => "abaAjax=cronograma"
		 				),
		 				array(	 "descricao" => "Questionário",
		 						"padrao" => ($_SESSION['obras2']['abaAjax'] == 'questionario' ? true : false) ,
		 						"url" => "obras2.php?modulo=principal/cadVistoriaEmpresa&acao={$_GET['acao']}",
		 						"parametro" => "abaAjax=questionario"
		 				),
		 				array(	 "descricao" => "Fotos",
		 						"padrao" => ($_SESSION['obras2']['abaAjax'] == 'fotos' ? true : false) ,
		 						"url" => "obras2.php?modulo=principal/cadVistoriaEmpresa&acao={$_GET['acao']}",
		 						"parametro" => "abaAjax=fotos"
		 				),
		 				array(	 "descricao" => "Tramitação",
		 						"padrao" => ($_SESSION['obras2']['abaAjax'] == 'tramitacao' ? true : false) ,
		 						"url" => "obras2.php?modulo=principal/cadVistoriaEmpresa&acao={$_GET['acao']}",
		 						"parametro" => "abaAjax=tramitacao"
		 				),
		 				array(	 "descricao" => "Restrições e Incoformidades",
		 						"padrao" => ($_SESSION['obras2']['abaAjax'] == 'restricao' ? true : false) ,
		 						"url" => "obras2.php?modulo=principal/cadVistoriaEmpresa&acao={$_GET['acao']}",
		 						"parametro" => "abaAjax=restricao"
		 				)
	  		   		 );

			 	$abaAjax->criaAba($arAba,'div_abas_ajax');
			 	?>
				</div>
			</td>
		</tr>
                <tr>
                    <td bgcolor="#c0c0c0" colspan="2" align="center">
                        <?php if($habilitadoPerfil):?>
                            <?php if($habilitado && $somenteLeitura == 'S'){ ?>
                                <input type="button" value="Salvar" id="salva_vistoria" style="cursor: pointer" onclick="enviaFormulario();">
                            <?php } ?>
                        <?php endif;?>
                            <input type="button" id="btnVoltar" value="Voltar" style="cursor: pointer" onclick="location.href='?modulo=principal/listVistoriaEmp&acao=A';">
                            <input type="button" id="btnVoltarOs" value="Voltar Para Lista de obras por OS" style="cursor: pointer" onclick="location.href='?modulo=principal/listaEmpreendimentoEmpresa&acao=A';">
                    </td>
                </tr>

	</table>
</form>

<?php
        $objObras = new Obras($obrid);
        $blockEdicao = $objObras->verificaObraVinculada();
        if($blockEdicao){
            echo '<script type="text/javascript">';
            echo " setTimeout(bloqueiaForm('formulario_supervisao_empresa'), 500);
                   function bloqueiaForm(idForm){
                      jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
                      jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                      jQuery('#btnVoltar').attr('disabled', false);
                      jQuery('#btnVoltarOs').attr('disabled', false);
                   }
                 ";
            echo '</script>';
        }
?>

<script type="text/javascript">
jQuery('#endcep').val( mascaraglobal('##.###-###', jQuery('#endcep').val()) );

function imprimirQuestionarioObraParalisada(){
    var url = "/obras2/obras2.php?modulo=principal/cadVistoriaEmpresa&acao=A&iqop=1";
    popup2 = window.open(
        url,
        "_blank",
        "width=1000,height=650,scrollbars=yes,scrolling=no,resizebled=no"
    );
}
function escondeQuestionarioObraParalisada(){
    jQuery('#divQuestionarioObraParalisada').attr('style', 'display: none');
}
function questionarioObraParalisada(){
    jQuery('#divQuestionarioObraParalisada').attr('style', 'display: inline');
}

function abreTodasPerguntasQuestionarioObraParalisada(value){
    if(value == 'S'){
        jQuery('#divPerguntasQuestionarioObraParalisada').attr('style', 'display: inline');
    }else if(value == 'N'){
        jQuery('#divPerguntasQuestionarioObraParalisada').attr('style', 'display: none');
        jQuery('#qoprespostadois').val('');
        jQuery('#qoprespostatres').val('');
        jQuery('#qoprespostaquatro_s').removeAttr('checked');
        jQuery('#qoprespostaquatro_n').removeAttr('checked');
        jQuery('#qoprespostacinco_s').removeAttr('checked');
        jQuery('#qoprespostacinco_n').removeAttr('checked');
    }else{
        jQuery('#qoprespostaum_s').removeAttr('checked');
        jQuery('#qoprespostaum_n').removeAttr('checked');
        jQuery('#qoprespostadois').val('');
        jQuery('#qoprespostatres').val('');
        jQuery('#qoprespostaquatro_s').removeAttr('checked');
        jQuery('#qoprespostaquatro_n').removeAttr('checked');
        jQuery('#qoprespostacinco_s').removeAttr('checked');
        jQuery('#qoprespostacinco_n').removeAttr('checked');
    }
}

function ctrlSituacaoObra( sobid ){

    if(sobid == '4'){
        questionarioObraParalisada();
    }else{
        escondeQuestionarioObraParalisada();
    }

    jQuery("#tr-paralisacao").hide();
    jQuery("select[name='tplid']").val("");
    if ( sobid == <?php echo SITUACAO_OBRA_EM_EXECUCAO ?> ){
            jQuery('#situacaoObraSubPergunta').show();
    } else if (sobid == <?php echo SITUACAO_OBRA_PARALISADA ?>) {
            jQuery("#tr-paralisacao").show();
    }else{
            jQuery('[name=sueacordo]:checked').attr('checked', false);
            jQuery('#situacaoObraSubPergunta').hide();
    }
}


function imprimirQuestionario(){
	return windowOpen( '?modulo=principal/cadVistoriaEmpresaImpressao&acao=A','blank',
						'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}
function imprimirQuestionarioRespondido(){
	return windowOpen( '?modulo=principal/cadVistoriaEmpresaImpressaoPreenchido&acao=A','blank',
						'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}
function ctrlImgObs(obj, sqtid){

	if ( jQuery(obj).attr('checked') ){
		jQuery('[id*=div_obs_sub_' + sqtid + ']').attr('class', 'divSubQuestaoNivel3');
	}else{
		jQuery('[id*=div_obs_sub_' + sqtid + ']').attr('class', 'divSubQuestaoNivel3_none');
	}
}

function ctrlObs(qstid, val){
	if ( jQuery("[name='qstid[" + qstid + "]']:checked").val() == 'f' && jQuery("[name='sqtid[" + qstid + "]']").size() > 0 ){
		jQuery('#div_obs_' + qstid).hide()
                .find('textarea')
                .val('');
		return;
	}

	if ( val == true ){
		jQuery('#div_obs_' + qstid).show();
	}else{
		jQuery('#div_obs_' + qstid).hide()
							  .find('textarea')
							  .val('');
	}
}

function ctrlSubPergunta(qstid, val){
	if ( val == true ){
		jQuery('[id*=div_subquestao_' + qstid + ']').attr('class', 'divSubQuestao');
	}else{
		jQuery('[id*=div_subquestao_' + qstid + ']').attr('class', 'divSubQuestao_none')
										  .find('input:file')
										  .val('')
										  .find('textarea')
										  .val('');
	}
}

var totalArq = new Array();
function addArquivo(qstid){
	totalArq[qstid] = (totalArq[qstid] ? totalArq[qstid] + 1 : 1);

	jQuery('<br>').appendTo('#div_arq_' + qstid + ' .divQuestaoNivel4_1');
	jQuery('#div_arq_' + qstid).find('[type=file]:eq(0)')
						  .clone()
						  .val('')
						  .attr('name', 'img_'+ qstid + '_' + totalArq[qstid])
						  .appendTo('#div_arq_' + qstid + ' .divQuestaoNivel4_1');
	jQuery('#totImg_' + qstid).val( totalArq[qstid] );

}

function inserirVistoriador(entid){
	var caminho_atual = '/obras2/obras2.php';

    if (entid){
        return windowOpen( caminho_atual + '?modulo=principal/inserir_vistoriador&acao=A&validaCPFProfissionalEmrpesa=1&busca=entnumcpfcnpj&entid=' + entid,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
    }else{
        return windowOpen( caminho_atual + '?modulo=principal/inserir_vistoriador&acao=A&validaCPFProfissionalEmrpesa=1&','blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
    }
}

function verificaEnd(opcao){
	if (opcao == 'S'){
		jQuery('[name*=endereco]').attr('readonly', 'readonly')
								  .addClass('disabled')
								  .val('');

		var endid = jQuery('#emp_endid').val();
		if ( endid ){
			var url = location.href;
			jQuery.ajax({
				type	: "POST",
				url		: url,
				data	: {endid : endid, ajax : 'buscaEndereco'},
				dataType: 'json',
				success	: function (data){
						jQuery('#endcep').val( mascaraglobal('##.###-###', data.endcep) );
						jQuery('#endlogradouro').val( data.endlogradouro );
						jQuery('#endnum').val( data.endnum );
						jQuery('#endcom').val( data.endcom );
						jQuery('#endbai').val( data.endbai );
						jQuery('#mundescricao').val( data.mundescricao );
						jQuery('#muncod').val( data.muncod );
						jQuery('#estuf').val( data.estuf );
					}
				});
		}
	}else{
		jQuery('[name*=endereco]').removeAttr('readonly')
								  .removeClass('disabled')
								  .val('');
	}
}

function calculoSupervisao(){
    var objClass     = this;
    var obCalc       = new Calculo();
    var percentTotal = new Number(0);
    var itemEdicao        = '';
    var vlrItemAnterior   = '';

    this.managerEdificacao = function (obj, tipo, obrid){
        obj  = obj || '';
        tipo = tipo || 'percentual';

        var vlrItemObra = jQuery( obj ).parents('tr:eq(0)')
                              .find('td:eq(1)')
                              .html();

        var vlrObj = mascaraglobal("[###.]###,##", obj.value);
        if (tipo == 'percentual' && obCalc.comparar(new String(vlrObj), '100,00', '>') ){
            obj.value = '100,00';
        }else if (obCalc.comparar(new String(vlrObj), vlrItemObra, '>') ){
            obj.value = vlrItemObra;
        }

        this.formatarNumeroMonetario( obj );
        this.calculaPercentLinha( obj, tipo, obrid );

        calculaTotalSupervisao( obrid );
    }

    this.managerEtapa = function (obj, obrid, tipo){
        obj = obj || '';
        tipo = tipo || 'percentual';

        var vlrItemObra = jQuery( obj ).parents('tr:eq(0)')
                              .find('td:eq(1)')
                              .html();
        var vlrObj = mascaraglobal("[###.]###,##", obj.value);
        if (tipo == 'percentual' && obCalc.comparar(new String(vlrObj), '100,00', '>') ){
            obj.value = '100,00';
        }else if (obCalc.comparar(new String(vlrObj), vlrItemObra, '>') ){
            obj.value = vlrItemObra;
        }

        this.formatarNumeroMonetario( obj );
        this.calculaPercentLinha( obj, tipo, obrid );

//        calculaNivelEdificacao( obrid );
        calculaTotalSupervisao( obrid );
    }

    this.managerDetalhamento = function (obj, obrid, icoid, tipo){

        obj = obj || '';
        tipo = tipo || 'percentual';

        var vlrItemObra = jQuery( obj ).parents('tr:eq(0)')
                              .find('td:eq(1)')
                              .html();

        var vlrObj = mascaraglobal("[###.]###,##", obj.value);

        if (tipo == 'percentual' && obCalc.comparar(new String(vlrObj), '100,00', '>') ){
            obj.value = '100,00';
        }else if (obCalc.comparar(new String(vlrObj), vlrItemObra, '>') ){
            obj.value = vlrItemObra;
        }

        this.formatarNumeroMonetario( obj );
        this.calculaPercentLinha( obj, tipo, obrid );

        calculaNivelEtapa( obrid, icoid );
//        calculaNivelEdificacao( obrid );
        calculaTotalSupervisao( obrid );
    }


    this.managerDetalhamentoFilho = function (obj, obrid, icoid, ditidPai, tipo){
        obj       = obj || '';
        tipo      = tipo || 'percentual';

        var vlrItemObra = jQuery( obj ).parents('tr:eq(0)')
                              .find('td:eq(1)')
                              .html();

        var vlrObj = mascaraglobal("[###.]###,##", obj.value);
        if (tipo == 'percentual' && obCalc.comparar(new String(vlrObj), '100,00', '>') ){
            obj.value = '100,00';
        }else if (obCalc.comparar(new String(vlrObj), vlrItemObra, '>') ){
            obj.value = vlrItemObra;
        }
        this.formatarNumeroMonetario( obj );
        this.calculaPercentLinha( obj, tipo, obrid );

        calculaNivelDetalhamento( obrid, icoid, ditidPai );
//        calculaNivelEtapa( obrid, icoid );
//        calculaNivelEdificacao( obrid );
        calculaTotalSupervisao( obrid );
    }


    this.calculaPercentLinha = function ( obj, tipo, obrid ){
        var vlrItemObra = jQuery( obj ).parents('tr:eq(0)')
                                  .find('td:eq(1)')
                                  .html();
        // CALCULA: "Valor Executado" a partir do "(%) Supervisão"
        if ( tipo == 'percentual' ){
            var calcVlrItemSup = obCalc.operacao(vlrItemObra, obj.value, '*', 20);
            calcVlrItemSup = obCalc.operacao(calcVlrItemSup, 100, '/', 20);
            jQuery( obj ).parents('tr:eq(0)')
                    .find('td:eq(10) input')
                    .val( this.formatarNumeroMonetario( {value : new Number( calcVlrItemSup ).toFixed(2)} ) );
        // CALCULA: "(%) Supervisão" a partir do "Valor Executado"
        }else{
            var calcPercItemSup = obCalc.operacao(obj.value, vlrItemObra, '/', 20);
            calcPercItemSup = obCalc.operacao(calcPercItemSup, 100, '*', 20);
            jQuery( obj ).parents('tr:eq(0)')
                    .find('td:eq(9) input')
                    .val( this.formatarNumeroMonetario( {value : new Number( calcPercItemSup ).toFixed(2)} ) );
        }

        // CALCULA: "(%) do Item já Executado sobre a Obra após Supervisão" a partir do "Valor Executado"
        var vlrItemSup = jQuery( obj ).parents('tr:eq(0)')
                                   .find('td:eq(10) input')
                                   .val();
        var vlrProjeto = jQuery('#vlrProjeto_' + obrid).val();
        var calcPercItemSupSobreObra = obCalc.operacao(vlrItemSup, vlrProjeto, '/', 20);
        calcPercItemSupSobreObra = obCalc.operacao(calcPercItemSupSobreObra, 100, '*', 20);
        jQuery( obj ).parents('tr:eq(0)')
                .find('td:eq(11) span')
                .html( this.formatarNumeroMonetario( {value : new Number( calcPercItemSupSobreObra ).toFixed(2)} ) );

        jQuery( obj ).parents('tr:eq(0)')
                .find('td:eq(11) input')
                .val( this.formatarNumeroMonetario( {value : new Number( calcPercItemSupSobreObra ).toFixed(2)} ) );



        // CALCULA: "diferença Valor Executado
        var vlrItemSup  = jQuery( obj ).parents('tr:eq(0)')
                                   .find('td:eq(10) input')
                                   .val();

        var vlrAnterior = jQuery( obj ).parents('tr:eq(0)')
                                   .find('td:eq(7) input[type="hidden"]')
                                   .val();


       	var calcDiferencaValor = obCalc.operacao(vlrItemSup, vlrAnterior, '-', 20);

       	var sinal = "";
       	if( new Number( calcDiferencaValor ) < 0 ){
	       	sinal = "-";
       	}else{
	       	sinal = "";
        }

        jQuery( obj ).parents('tr:eq(0)')
                .find('td:eq(13) span')
                .html( sinal+""+this.formatarNumeroMonetario( {value : new Number( calcDiferencaValor ).toFixed(2)} ) );

        jQuery( obj ).parents('tr:eq(0)')
                .find('td:eq(13) input')
                .val( sinal+""+this.formatarNumeroMonetario( {value : new Number( calcDiferencaValor ).toFixed(2)} ) );


        // CALCULA: "diferença % Executado
        var percItemSup  = jQuery( obj ).parents('tr:eq(0)')
                                   .find('td:eq(9) input')
                                   .val();

        var percAnterior = jQuery( obj ).parents('tr:eq(0)')
                                   .find('td:eq(7) input[type="text"]')
                                   .val();

        var calcDiferencaPerc = obCalc.operacao(percItemSup, percAnterior, '-', 20);

        jQuery( obj ).parents('tr:eq(0)')
                .find('td:eq(14) span')
                .html(  (new Number( calcDiferencaPerc ).toFixed(2)).replace(".",",") );

        jQuery( obj ).parents('tr:eq(0)')
                .find('td:eq(14) input')
                .val(  (new Number( calcDiferencaPerc ).toFixed(2)).replace(".",",") );


    }

    this.formatarNumeroMonetario = function ( obj ){
        var valor = new String( obCalc.retiraZeroEsquerda( new String(obj.value) ) );

        if ( valor.length == 2 ){
            valor = '0' + valor;
        }else if ( valor.length == 1 ){
            valor = '00' + valor;
        }else if ( valor.length == 0 ){
            valor = '000';
        }

        obj.value = mascaraglobal("[###.]###,##", valor);
        return obj.value;
    }

    this.managerMetaFisica = function ( obj, limiteMetaFisica ){
        obj = obj || '';

        var vlrObj = this.formatarNumeroMonetario( {value : new String( obj.value )} );
        obj.value = vlrObj;
        if (obCalc.comparar(new String(vlrObj), limiteMetaFisica, '>') ){
            obj.value = this.formatarNumeroMonetario( {value : new Number( limiteMetaFisica ).toFixed(2) } );
        }

    }

    function calculaNivelEdificacao( obrid ){
        var idTrEdicacao   = 'tr_item_edificacao_';
        var idTrEtapa      = 'tr_item_etapa_';
        var vlrEtapaSupTot = 0;

        jQuery('[id^=' + idTrEtapa + obrid + '_]').each(function (i, obj){
        	vlrEtapaSupTot = obCalc.operacao(vlrEtapaSupTot, jQuery( obj ).find('td:eq(10) input').val(), '+', 20);
        });

        jQuery( '#' + idTrEdicacao + obrid ).find('td:eq(10) input')
                                       .val( objClass.formatarNumeroMonetario( {value : new Number( vlrEtapaSupTot ).toFixed(2)} ) );
        objClass.managerEdificacao(jQuery( '#' + idTrEdicacao + obrid ).find('td:eq(10) input')[0] , 'numerico');
    }

    function calculaNivelEtapa( obrid, icoid ){
        var idTrEtapa             = 'tr_item_etapa_';
        var idTrDetalhamento      = 'tr_item_detalhamento_';
        var vlrDetalhamentoSupTot = 0;
        jQuery('[id^=' + idTrDetalhamento + obrid + '_' + icoid + '_]').each(function (i, obj){
        	vlrDetalhamentoSupTot = obCalc.operacao(vlrDetalhamentoSupTot, jQuery( obj ).find('td:eq(10) input').val(), '+', 20);
        });

        jQuery( '#' + idTrEtapa + obrid + '_' + icoid ).find('td:eq(10) input')
                                                  .val( objClass.formatarNumeroMonetario( {value : new Number( vlrDetalhamentoSupTot ).toFixed(2)} ) );

        objClass.managerEtapa(jQuery( '#' + idTrEtapa + obrid + '_' + icoid ).find('td:eq(10) input')[0], obrid, 'numerico');
    }

    function calculaNivelDetalhamento( obrid, icoid, ditidPai ){
        var idTrDetalhamento      = 'tr_item_detalhamento_';
        var idTrDetalhamentoFilho = 'tr_item_detalhamento_filho_';

        var vlrDetalhamentoFilhoSupTot   = 0;

        jQuery('[id^=' + idTrDetalhamentoFilho + obrid + '_' + icoid + '_' + ditidPai + '_]').each(function (i, obj){
        	vlrDetalhamentoFilhoSupTot = obCalc.operacao(vlrDetalhamentoFilhoSupTot, jQuery( obj ).find('td:eq(10) input').val(), '+', 20);
        });

        jQuery( '#' + idTrDetalhamento + obrid + '_' + icoid + '_' + ditidPai ).find('td:eq(10) input')
                                                  .val( objClass.formatarNumeroMonetario( {value : new Number( vlrDetalhamentoFilhoSupTot ).toFixed(2)} ) );

        objClass.managerDetalhamento(jQuery( '#' + idTrDetalhamento + obrid + '_' + icoid + '_' + ditidPai ).find('td:eq(10) input')[0], obrid, icoid, 'numerico');
    }

    function calculaTotalSupervisao( obrid ){
        var idTrEtapa      		= 'tr_item_etapa_' + obrid;
        var idTrTotal           = 'tr_total_supervisao_' + obrid;
        var vlrProjeto          = jQuery('#vlrProjeto_' + obrid).val();
        var vlrEdificacaoSupTot = 0;
        var percSupTot          = 0;

        jQuery('[id^=' + idTrEtapa + '_]').each(function (i, obj){
          vlrEdificacaoSupTot = obCalc.operacao(vlrEdificacaoSupTot, jQuery( obj ).find('td:eq(10) input').val(), '+', 20);
        });

        percSupTot = obCalc.operacao(vlrEdificacaoSupTot, vlrProjeto, '/', 20);
        percSupTot = obCalc.operacao(percSupTot, 100, '*', 20);

        jQuery('#' + idTrTotal).find('td:eq(11)')
                        .html( objClass.formatarNumeroMonetario( {value : new Number( percSupTot ).toFixed(2)} ) );
    }

}

calculoSupervisao = new calculoSupervisao();

function enviaFormulario(){

	var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
	var validacao = true;

	if ( jQuery('#sosid').val() == '' ){
		mensagem += 'OS \n';
		validacao = false;
	}

        if(jQuery('#sobid').val() === '4'){

            <?php
                $supEmp = new SupervisaoEmpresa( $sueid );
                $dataOs = $supEmp->pegaDataOsSupEmpresa($sueid);
                $dataOs = explode('-', $dataOs);
                $dataOs = ''.$dataOs[0].', '.$dataOs[1].', '.$dataOs[2];
            ?>

            var dataOs     = new Date(<?php echo $dataOs; ?>);
            var dataLimite =  new Date(2014, 6, 9);

            if (dataOs > dataLimite) {
                if(jQuery('[name=qoprespostaum]:checked').val() == 'S'){
                    if(isNaN(jQuery('#qoprespostadois').val())){
                        mensagem += 'A pergunta 2 do Questionário deve ser preenchida com números. \n';
                        validacao = false;
                    }
                    if(jQuery('#qoprespostadois').val() == ''){
                        mensagem += 'A pergunta 2 do Questionário - Supervisão Empresa - Obra Paralisada deve ser respondida. \n';
                        validacao = false;
                    }
                    if(isNaN(jQuery('#qoprespostatres').val())){
                        mensagem += 'A pergunta 3 do Questionário deve ser preenchida com números. \n';
                        validacao = false;
                    }
                    if(jQuery('#qoprespostatres').val() == ''){
                        mensagem += 'A pergunta 3 do Questionário - Supervisão Empresa - Obra Paralisada deve ser respondida. \n';
                        validacao = false;
                    }
                    if(typeof(jQuery('[name=qoprespostaquatro]:checked').val()) == 'undefined'){
                        mensagem += 'A pergunta 4 do Questionário - Supervisão Empresa - Obra Paralisada deve ser respondida. \n';
                        validacao = false;
                    }
                    if(typeof(jQuery('[name=qoprespostacinco]:checked').val()) == 'undefined'){
                        mensagem += 'A pergunta 5 do Questionário - Supervisão Empresa - Obra Paralisada deve ser respondida. \n';
                        validacao = false;
                    }
                }else if(typeof(jQuery('[name=qoprespostaum]:checked').val()) == 'undefined'){
                        mensagem += 'A pergunta 1 do Questionário - Supervisão Empresa - Obra Paralisada deve ser respondida. \n';
                        validacao = false;
                }
            }
        }

	if( !validaNumeroCaracteresTextareas( ['qtsobs','rsqobs'], 1000 ) ){
		mensagem = 'A quantidade de caracteres informada em algum campo de texto extrapola o limite estabelecido.';
		validacao = false;

	}

	if ( !validacao ){
		alert(mensagem);
	}else{
		jQuery('#formulario_supervisao_empresa').submit();
	}

}

function removerFotoGaleria(foto, pk)
{
	var tableta = jQuery(foto).parent().parent().attr('id').split('_');
	var campo   = tableta[1];
	tableta	    = tableta[0]
	var id 	    = jQuery(foto).parent().parent().attr('name');
	var arqid   = jQuery(foto).parent().attr('name');

	if( id && arqid ){
		var url = window.location + "&ajax=excluirFoto&tabela="+tableta+"&campo="+campo+"&id="+id+"&arqid="+arqid+"&pk="+pk;

		jQuery.ajax({
			url: url,
			success: function(data) {
		  }
		});
	}
	id = jQuery(foto).parent().attr('id');
	if( jQuery('[name="'+arqid+'"]').size() < 3 ){
		jQuery('#fotos_galeria').find('[id="'+id+'"]').removeClass("f_selected");
	}
	jQuery(foto).parent().remove();
}

function incluirFotos( sueid ){
	janela = window.open('?modulo=principal/popupFotosSupervisaoEmpresa&acao=A&sueid=' + sueid, 'inserirFotosSupervisao', 'menubar=no,location=no,resizable=no,scrollbars=no,status=yes,width=750,height=700' );
	janela.focus();
}

function abrirGaleria(arqid,pagina,obj)
{
	var tableta = jQuery(obj).parent().parent().attr('id').split('_');
	var campo 	= tableta[1];
//	alert(campo);
//	tabela		= tableta[0]
	var id 		= jQuery(obj).parent().parent().attr('name');
//	if( tabela == 'fotos' ){
//		tabela = '';
//	}
	window.open("../slideshow/slideshow/obras2_galeriaSupervisao.php?&arqid=" + arqid + "&" + campo + "=" + id+"&pagina=" + pagina,"imagem","width=850,height=600,resizable=yes");
	//window.open("../slideshow/slideshow/verimagem.php?arqid=" + arqid,"imagem","width=850,height=600,resizable=yes");
}

jQuery(document).ready(function() {

    jQuery('#qoprespostadois').val( mascaraglobal('####', jQuery('#qoprespostadois').val()) );
    jQuery('#qoprespostatres').val( mascaraglobal('####', jQuery('#qoprespostatres').val()) );

    if(jQuery('#sobid').val() === '4'){
        questionarioObraParalisada();
    }

    jQuery(".ico_spitotitemsupervisor input").keyup(function () {

        var obCalc = new Calculo();
        var qtd = parseInt(jQuery(this).val());
        var maxQtd = parseInt(jQuery(this).parents('tr:eq(0)').find('td:eq(3)').text().trim());
        var total = jQuery('#tr_total_supervisao_d td:eq(1)').text().trim();
        var valorItem = jQuery(this).parents('tr:eq(0)').find('td:eq(1)').text().trim();

        if(qtd > maxQtd){
            qtd = maxQtd;
            jQuery(this).val(maxQtd);
            alert('A quantidade não pode ultrapassar a quantidade definida.');
        }

        // Total de acordo com o total
        valor = obCalc.operacao(valorItem, qtd,'*',20);
        valor = obCalc.operacao(valor, 100,'*',20);
        valor = obCalc.operacao(valor, total,'/',20);
        valor = calculoSupervisao.formatarNumeroMonetario( {value : new Number( valor ).toFixed(2)} );
        valor = valor + '<input type="hidden" name="ico_spipercsobreobrainfsupervisor[<?php echo $obrid?>][]" value="'+valor+'">';

        jQuery(this).parents('td:eq(0)').next().next().next().html(valor);

        // Total do item
        valor = (qtd * 100) / maxQtd;
        jQuery(this).parents('td:eq(0)').next().find('input').val(calculoSupervisao.formatarNumeroMonetario( {value : new Number( valor ).toFixed(2)} ));

        valor = obCalc.operacao(qtd, valorItem, '*',2);
        jQuery(this).parents('td:eq(0)').next().next().find('input').val(calculoSupervisao.formatarNumeroMonetario( {value : new Number( valor ).toFixed(2)} ));

        valor = 0;
        jQuery('.totpercentitem').each(function(i){
            valor = obCalc.operacao(valor, jQuery(this).text().trim(),'+',2);
        });
        jQuery('#td_exec_obra_d').html(valor);

    });
});
</script>

<?php

function conteudoDivQuestionarioObraParalisada($tipo_impressao = 'tela') {
    global $db;
    $resp = array();
    if (!empty($_SESSION['obras2']['sueid'])) {
        $sql_quest = 'SELECT *
                      FROM obras2.questionarioobraparalisada
                      WHERE sueid = ' . $_SESSION['obras2']['sueid'];
        $resp = $db->pegaLinha($sql_quest);
        $resp = (empty($resp)) ? array() : $resp;
    }

    if($tipo_impressao == 'impressao'){

        echo '
                <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                <script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
                <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
                <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
                <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
                <script language="JavaScript" src="../includes/funcoes.js"></script>
                <script>jQuery.noConflict();</script>
                <script type="text/javascript">

                    function lock(){
                        jQuery("textarea, input, select").attr("disabled", "true");
                        jQuery("[value=Fechar]").removeAttr("disabled");
                    };

                    function imprimirQuestionarioObraParalisada(){
                        var url = "/obras2/obras2.php?modulo=principal/cadVistoriaEmpresa&acao=A&iqop=1";
                        popup2 = window.open(
                            url,
                            "_blank",
                            "width=1000,height=650,scrollbars=yes,scrolling=no,resizebled=no"
                        );
                    }
                    function escondeQuestionarioObraParalisada(){
                        jQuery(\'#divQuestionarioObraParalisada\').attr(\'style\', \'display: none\');
                    }
                    function questionarioObraParalisada(){
                        jQuery(\'#divQuestionarioObraParalisada\').attr(\'style\', \'display: inline\');
                    }

                    function abreTodasPerguntasQuestionarioObraParalisada(value){
                        if(value == \'S\'){
                            jQuery(\'#divPerguntasQuestionarioObraParalisada\').attr(\'style\', \'display: inline\');
                        }else if(value == \'N\'){
                            jQuery(\'#divPerguntasQuestionarioObraParalisada\').attr(\'style\', \'display: none\');
                            jQuery(\'#qoprespostadois\').val(\'\');
                            jQuery(\'#qoprespostatres\').val(\'\');
                            jQuery(\'#qoprespostaquatro_s\').removeAttr(\'checked\');
                            jQuery(\'#qoprespostaquatro_n\').removeAttr(\'checked\');
                            jQuery(\'#qoprespostacinco_s\').removeAttr(\'checked\');
                            jQuery(\'#qoprespostacinco_n\').removeAttr(\'checked\');
                        }else{

                        }
                    }
                </script>
             ';

        $html_cabecalho = '<tr>
                                <td colspan="2">
                                    '.monta_cabecalho_relatorio(100).'
                                </td>
                            </tr>';
    }else{
        $html_cabecalho = '';
    }


    ?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" border="0" style="width:100%">
    <tbody>
        <?php echo $html_cabecalho;?>
        <tr>
            <td colspan="2">
                <div style="float:right">
                    <?php if(empty($_REQUEST['iqop'])){ ?>
                        <img title="Imprimir questionário" onclick="imprimirQuestionarioObraParalisada()" src="../imagens/print.png" class="link">
                    <?php }?>
                </div>
            </td>
        </tr>
        <tr>
            <td colspan="2">
                <fieldset>
                    <legend>Questionário - Supervisão Empresa - Obra Paralisada</legend>
                    <p>
                        1 - O município tem interesse em finalizar a obra?
                        <br />
                        <input type="radio" name="qoprespostaum" id="qoprespostaum_s" value="S" <?php echo ($resp['qoprespostaum'] != 'S' ? '' : 'checked="checked"') ?> onclick="abreTodasPerguntasQuestionarioObraParalisada(this.value)"> Sim &nbsp;
                        <input type="radio" name="qoprespostaum" id="qoprespostaum_n" value="N" <?php echo ($resp['qoprespostaum'] != 'N' ? '' : 'checked="checked"') ?> onclick="abreTodasPerguntasQuestionarioObraParalisada(this.value)" > Não
                    </p>
                    <!--<div id="divPerguntasQuestionarioObraParalisada" style="display: none">-->
                        <p>
                            2 - Dentro de que prazo o município irá concluir a obra? (N° de dias)
                            <br />
                            <input type="text" name="qoprespostadois" id="qoprespostadois" value="<?php echo $resp['qoprespostadois'] ?>" maxlength="3" onkeyup="mascaraglobal('###', this.value)">
                        </p>
                        <p>
                            3 - O município já elaborou o novo processo licitatório para reinício e conclusão da obra, se sim, qual o prazo de encerramento, se não, qual o prazo de lançamento do Edital? (N° de dias)
                            <br />
                            <input type="text" name="qoprespostatres" id="qoprespostatres" value="<?php echo $resp['qoprespostatres'] ?>" maxlength="3" onkeyup="mascaraglobal('###', this.value)">
                        </p>
                        <p>
                            4 - O município irá concluir a obra por administração direta?
                            <br />
                            <input type="radio" name="qoprespostaquatro" id="qoprespostaquatro_s" value="S" <?php echo ($resp['qoprespostaquatro'] != 'S' ? '' : 'checked="checked"') ?> > Sim &nbsp;
                            <input type="radio" name="qoprespostaquatro" id="qoprespostaquatro_n" value="N" <?php echo ($resp['qoprespostaquatro'] != 'N' ? '' : 'checked="checked"') ?> > Não
                        </p>
                        <p>
                            5 - O município precisa de orientação sobre como proceder para der continuidade aos serviços?
                            <br />
                            <input type="radio" name="qoprespostacinco" id="qoprespostacinco_s" value="S" <?php echo ($resp['qoprespostacinco'] != 'S' ? '' : 'checked="checked"') ?> > Sim &nbsp;
                            <input type="radio" name="qoprespostacinco" id="qoprespostacinco_n" value="N" <?php echo ($resp['qoprespostacinco'] != 'N' ? '' : 'checked="checked"') ?> > Não
                        </p>
                    <!--</div>-->
                </fieldset>
            </td>
        </tr>
        <?php if($tipo_impressao == 'impressao'){?>
        <tr>
            <td colspan="2">
                <center><input type="button" value="Fechar" style="cursor: pointer" onclick="window.close()"></center>
            </td>
        </tr>
        <script type="text/javascript">
            lock();
        </script>
        <?php }?>
    </tbody>
</table>
<?php
}

?>

  <script>
      
    var perguntasCaimento = [217, 218, 219];
    
    var chagePerguntasCaimento = function() {
        var idChecked = $1_11(this).attr('name').replace('qstid[', '').replace(']', '');
        
        for (var i = 0; i < perguntasCaimento.length; i++) {
            if (perguntasCaimento[i] != idChecked && $1_11('input[name="qstid[' + idChecked + ']"]:checked').val() == 'f') {
                if ($1_11('input[name="qstid[' + perguntasCaimento[i] + ']"]:checked').val() == 'f') {
                    alert('Somente é possível marcar  NÃO para uma das questões sobre caimento (itens 2.3, 2.4 e 2.5)');
                    $1_11('input[name="qstid[' + idChecked + ']"]').each(function() {
                        if($1_11(this).val() == "n"){
                            $1_11(this).prop('checked', true);
                        }                                                               
                    });
                }
            }
        }
    };
      
    for (var i = 0; i < perguntasCaimento.length; i++) {
        console.log(perguntasCaimento[i]);
        $1_11(document).on('change', 'input[name="qstid[' + perguntasCaimento[i] + ']"]', chagePerguntasCaimento);
    }
    
    
     var perguntasArrimo = [220, 221, 222];
    
    var chagePerguntasArrimo = function() {
        var idChecked = $1_11(this).attr('name').replace('qstid[', '').replace(']', '');
        
        for (var i = 0; i < perguntasArrimo.length; i++) {
            if (perguntasArrimo[i] != idChecked && $1_11('input[name="qstid[' + idChecked + ']"]:checked').val() == 'f') {
                if ($1_11('input[name="qstid[' + perguntasArrimo[i] + ']"]:checked').val() == 'f') {
                    alert('Somente é possível marcar NÃO para uma das questões sobre muro de arrimo (itens 2.6, 2.7 e 2.8)');
                    $1_11('input[name="qstid[' + idChecked + ']"]').each(function() {
                        if($1_11(this).val() == "n"){
                            $1_11(this).prop('checked', true);
                        }                                                               
                    });
                }
            }
        }
    };
      
    for (var i = 0; i < perguntasArrimo.length; i++) {
        console.log(perguntasArrimo[i]);
        $1_11(document).on('change', 'input[name="qstid[' + perguntasArrimo[i] + ']"]', chagePerguntasArrimo);
    }
    
     $1_11(document).on('change', '#sobid',function(){
         
     
     if($1_11(this).val() == 1){
         $1_11("#trruralurbana").show();
     }
     else{
         $1_11("#trruralurbana").hide();
     }
      
      if($1_11(this).val() == 5){
          $1_11("#sueprojetofnde").hide();
           $1_11("#unidade_funcionamento").hide();
           $1_11("#total_percentual").hide();
          
      }else{
          
          $1_11("#sueprojetofnde").show();
          $1_11("#unidade_funcionamento").show();
          $1_11("#total_percentual").show();
      }   
      
      
      
      
                  
     });

 </script>