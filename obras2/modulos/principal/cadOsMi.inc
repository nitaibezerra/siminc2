<?php

if(!is_numeric($_SESSION['obras2']['obrid'])){
    $_SESSION['obras2']['obrid'] = '';
}

if($_SESSION['obras2']['obrid'] == '' && $_GET['obrid'] != ''){
    $_SESSION['obras2']['obrid'] = $_GET['obrid'];
    $obr = new Obras($_SESSION['obras2']['obrid']);
    $_SESSION['obras2']['empid'] = $obr->empid;
}
if($_SESSION['obras2']['orgid'] == ''){
    $_SESSION['obras2']['orgid'] = 3;
}
if($_SESSION['obras2']['empid'] == '' ){
    $obr = new Obras($_SESSION['obras2']['obrid']);
    $_SESSION['obras2']['empid'] = $obr->empid;
}
if($_SESSION['obras2']['empid'] == '' ){
    $_SESSION['obras2']['empid'] = $_SESSION['obras2']['obrid'];
}

// empreendimento || obra || orgao
verificaSessao( 'obra' );

$obra = new Obras();
$dados = $obra->pegaDadosCompletoPorObrid($_SESSION['obras2']['obrid']);
if( empty($dados) ){
    die('<script type="text/javascript">
                alert("Obra não encontrada.");
                if(history.length > 1){
                    history.back(-1);
                }else{
                    window.close();
                }
          </script>');
}

$tomid = '';

$_SESSION['tomid'] = $_GET['tomid'] != '' ? $_GET['tomid'] : $_SESSION['tomid'] ;
$osmid = $_REQUEST['osmid'];

$empid = $_SESSION['obras2']['empid'];
$obrid = $_SESSION['obras2']['obrid'];
$tomid = $_SESSION['tomid'];
//$tomid = $_GET['tomid'];

$ordemServico = new OrdemServicoMI();
if (!empty($osmid)) {
    $ordemServico->carregarPorOsmid($osmid);
}elseif(!empty($tomid)){
    $ordemServico->carregarPorObridETomid($obrid, $tomid );
} else{
    $ordemServico->carregarPorObrid( $obrid );
}

//if(!checaOsImplantacao($obrid)){
//    echo '<script type="text/javascript">
//                alert("É necessário um OS de implantação concluida.");
//                if(history.length > 1){
//                    history.back(-1);
//                }else{
//                    window.close();
//                }
//          </script>';
//}


$estid = pegaEstadoOsMi($ordemServico->docid);
if($_REQUEST['nova']){
    unset ($_REQUEST['req']);
    $ordemServico->clearDados();
}

$somenteLeitura = 'S';
if($ordemServico->docid && $estid != ESDID_OS_MI_CADASTRAMENTO && !$_REQUEST['nova']){
    $somenteLeitura = 'N';
}
//demanda 311298 -- O perfil call center não pode alterar/inserir nenhuma informação.
if( possui_perfil(array(PFLCOD_CALL_CENTER))){
    $perfCallCenter = TRUE;
    $somenteLeitura = 'S';
}
switch ($_REQUEST['req']){
	case 'salvar':
            
                $docdsc = "Fluxo de OS MI do módulo Obras II - obrid " . $obrid;
                // cria documento do WORKFLOW
                $docid = wf_cadastrarDocumento( TPDID_OS_MI, $docdsc );
		$dados = $_POST;
                
                if(empty($dados['osmid'])){
                      $ordemServico->clearDados();  
                      $dados['tomid'] = ($_GET['tomid'] != '') ? $_GET['tomid'] : $_SESSION['tomid'];
                }
                
                $obra = new Obras($obrid);
                $esdid = pegaEstadoObra($obra->docid);
                if ($dados['tomid'] == 1 && ($esdid == ESDID_OBJ_OS_RECUSADA)) {
                    wf_alterarEstado($obra->docid, 2513, '', array());
                }
        $obra = new Obras($obrid);

		$dados['docid'] 	= $docid;
		$dados['usucpf'] 	= $_SESSION['usucpf'];
		$dados['obrid']  	= $obrid;
		$dados['osmdtcadastro'] = formata_data_sql( $_POST['osmdtcadastro'] );
		$dados['osmdtinicio'] 	= formata_data_sql( $_POST['osmdtinicio'] );
		$dados['osmdttermino'] 	= formata_data_sql( $_POST['osmdttermino'] );
		$dados['osmprazo'] = $osmprazo = ( $tomid == 1 ) ? $obra->pegaPrazoMI() : $dados['osmprazo'];

		$arqid   = ($dados['arqid'] ? $dados['arqid'] : NULL);
                $arquivo = $_FILES["arquivo"];
                if ( $_FILES["arquivo"] && $arquivo["name"] && $arquivo["type"] && $arquivo["size"] ){
                    include_once APPRAIZ."includes/classes/fileSimec.class.inc";

                    $file = new FilesSimec(null, null,"obras2");
                    $file->setPasta('obras2');
                    $file->setUpload(null, 'arquivo', false);
                    $arqid = $file->getIdArquivo();
                }
                
                $dados['arqid'] = $arqid;
                $ordemServico->popularDadosObjeto($dados)->salvar();
                $db->commit();
//                ver($dados,d); 
                die("<script>
                        alert('Operação realizada com sucesso!');
                        location.href='?modulo=principal/cadOsMi&acao=A&tomid=$tomid'; 
                 </script>");
                
	case 'downloadArquivo':
		$arqid = $_GET['arqid'];
		if ( $arqid ){
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			$file 		  = new FilesSimec(null, null, "obras2");
			$file->getDownloadArquivo($arqid);
		}
		
		die('<script>
                            location.href=\'?modulo=principal/cadOsMi&acao=A\';
                     </script>');
}

extract( $ordemServico->getDados() );

// Recarrega os dados em caso de não ter OS cadastrada
$empid = ($empid ? $empid : $_SESSION['obras2']['empid']);
$obrid = ($obrid ? $obrid : $_SESSION['obras2']['obrid']);

?>
<html>

<script type="text/javascript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript">
function novaOS(){
	$('#nova').val('true');
	$('#formulario').submit();
}

function dataTerminoExecucao(){
	var dtInicioExec     = jQuery('#osmdtinicio').val(); //document.formulario.osmdtinicio.value;
	var diaPrazoExecucao = jQuery('#osmprazo').val(); //document.formulario.osmprazo.value;
	var dtTerminoExec    = "";
	    tomid = $('#tomid').val();
        if( (diaPrazoExecucao > 30 ) && (tomid == 2 || tomid == 3) ){
            alert('O prazo para execução não pode ser superior a 30 dias.');
            jQuery('#osmprazo').val('');
            jQuery('#osmprazo').focus();
            
        }else{
            if (dtInicioExec && diaPrazoExecucao){
                    var obDt = new Data();
                    dtTerminoExec = obDt.dtAddDia(dtInicioExec, diaPrazoExecucao);
            }
            jQuery('#osmdttermino').val( dtTerminoExec );            
        }
        
}

function validarFormOS(){
	var msg = new Array();

	if ( $('#tomid').val() == '' ){
		msg.push('Tipo');
	}
	
	if ( $('#osmdtcadastro').val() == '' ){
		msg.push('Data da OS');
	}
	
	if ( $('#osmdtinicio').val() == '' ){
		msg.push('Data de início da execução');
	}
	
	if ( $('#osmprazo').val() == '' ){
		msg.push('Prazo de execução (dias)');
	}
	
	if ( $('#osmdttermino').val() == '' ){
		msg.push('Data de término da execução');
	}
	
	if ( $('#arqid').val() == '' && $('#arquivo').val() == '' ){
		msg.push('Ordem de Serviço (anexo)');
	}
	
	if ( msg.length > 0 ){
		msg = msg.join('\n');
		msg = 'Os seguintes campos são de preenchimento obrigatório:\n' + msg;
		
		alert( msg );
		return false;
	}
	
	$('#req').val('salvar');
	$('#formulario').submit();
}
</script>

<body marginheight="0" marginwidth="0">
<br/>
<?php
$db->cria_aba(ID_ABA_OS_MI,$url, '&tomid=' . $tomid . '&osmid=' . $osmid);

echo cabecalhoObra($obrid);

$chk_exec = '';
$chk_sond = '';
$chk_prim = '';

switch ($tomid) {
    case 1:
        $tp_os = 'de Execução';
        $chk_exec = 'checked="checked"';
        break;
    case 2:
        $tp_os = 'de Sondagem';
        $chk_sond = 'checked="checked"';
        break;
    case 3:
        $tp_os = 'do Projeto de Implantação';
        $chk_prim = 'checked="checked"';
        break;
}

$osMi = new OrdemServicoMI();
$imgs = $osMi->getBtnSinalizacaoOsMi($obrid);

$htm = '
        <table align="center" bgcolor="#f5f5f5" border="0" class="Tabela" cellpadding="3" cellspacing="1">
                <tr>
                    <td class="SubTituloDireita" width="20%"><b>Tipo de OS:</b></td>
                    <td>
                          <input type="radio" value="2" name="radio_tipo_os" id="radio_tipo_os_sond" ' . $chk_sond . '> '.$imgs['sondagem'].   ' OS de Sondagem &nbsp; 
                          <input type="radio" value="3" name="radio_tipo_os" id="radio_tipo_os_prim" ' . $chk_prim . '> '.$imgs['implantacao'].' OS do Projeto de Implantação &nbsp;
                          <input type="radio" value="1" name="radio_tipo_os" id="radio_tipo_os_exec" ' . $chk_exec . '> '.$imgs['execucao'].   ' OS de Execução &nbsp; 
                    </td>
                </tr>
        </table>
        
        ';

    echo $htm;

monta_titulo( 'Cadastro de OS', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.');

?>

<script type="text/javascript">        
    $('input:radio[name=radio_tipo_os]').click(function() {
       var tomid = $("input:radio[name=radio_tipo_os]:checked" ).val();
       //Linha colocada para no caso de ser acionado o history.back, o radio correto estará marcado
       $('input:radio[name=radio_tipo_os][value=<?php echo $tomid;?>]').attr('checked', true);
       var url = 'obras2.php?modulo=principal/cadOsMi&acao=A&tomid='+tomid;
       window.location = url;
    });                
</script>

<form id="formulario" name="formulario" method=post enctype="multipart/form-data" >
    <input type="hidden" name="osmid" id="osmid" value="<?php echo $osmid?>" />
	<input type="hidden" name="req" id="req" value="" />
	<input type="hidden" name="nova" id="nova" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
        <?php
        if ( $osmid ):
        ?>	
            <tr>
                <td class="SubTituloDireita" width="20%">Cadastrado por:</td>
                <td>
                <?php
                    echo $ordemServico->pegaCadastrantePorOsmid( $osmid );
                ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="20%">Data de cadastro:</td>
                <td>
                <?php
                    echo formata_data( $osmdtcadastro );
                ?>
                </td>
            </tr>
        <?php
        endif;
        ?>	
            <tr>
                <td class="SubTituloDireita" width="20%" valign="top">Tipo:</td>
                <td valign="top">
                <?php
                    $tipoOsMi = new TipoOsMi();
                    $leitura = $somenteLeitura;

                    if($tomid){
//                        if($_GET['tomid']){
                        $leitura = 'N';
                    }
                    $db->monta_combo( 'tomid', $tipoOsMi->listaCombo(), $leitura, 'Selecione', '', '', '', '115', 'S', 'tomid', '', $tomid);   
                ?>
                </td>
                <td rowspan="7" valign="top" align="center">
                <?php
                    // Barra de estado WORKFLOW
                    if($perfCallCenter){
                        wf_desenhaBarraNavegacao($docid, array('osmid' => $osmid), array('acoes' => true));
                    }else{
                        if( $docid ):
                            wf_desenhaBarraNavegacao($docid, array('osmid' => $osmid));
                        endif;   
                    }
                ?>
                </td>
            </tr>
	    <tr>
	        <td align='right' class="SubTituloDireita" valign="top">Data da OS:</td>
	        <td valign="top">
	        <?php $osmdtcadastro = formata_data($osmdtcadastro); ?>
		    <?php echo  campo_data2( 'osmdtcadastro', 'S', $somenteLeitura, '', 'N' ); ?>
		    </td>
	    </tr>    		
	    <tr>
	        <td align='right' class="SubTituloDireita" valign="top">Data de início da execução:</td>
	        <td valign="top">
	        <?php $osmdtinicio = formata_data($osmdtinicio); ?>
		    <?php echo  campo_data2( 'osmdtinicio', 'S', $somenteLeitura, '', 'N', '', 'dataTerminoExecucao();',null,'dataTerminoExecucao();' ); ?>
		    </td>
	    </tr> 
		<tr>
			<td class="subtitulodireita" valign="top">Prazo de execução (dias):</td>
            <?
            // Quando OS de execução, pegar prazos definidos
            $somenteLeituraPrazo = $somenteLeitura;
            if($tomid == 1){
                $obra = new Obras($obrid);
                $osmprazo = $obra->pegaPrazoMI();
                $somenteLeituraPrazo = 'N';
            }
            ?>
			<td valign="top"><?php echo campo_texto('osmprazo', 'S', $somenteLeituraPrazo, '', 5, 10, '[#]', '', 'right', '', 0, 'id="osmprazo" onchange="dataTerminoExecucao();"', 'dataTerminoExecucao();', $osmprazo, '' ); ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita" valign="top">Data de término da execução:</td>
			<td valign="top"><?php echo campo_data2('osmdttermino', 'N', 'N','Data de término da execução', 'S', '', '', $osmdttermino, '', '', 'osmdttermino'); ?></td>
		</tr>

        <?php if (($estid == ESDID_OS_MI_REPROVADA) && isset($osmid)): ?>

        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%" valign="top" >Motivo da recusa:</td>
            <td valign="top">
                <?php
                $recusa = new ObraRecusaOS();
                $motivo = $recusa->pegaMotivo($osmid);

                if(!empty($motivo))
                    echo $motivo['trodescricao'];
                else
                    echo 'Outros';
                ?>
            </td>
        </tr>

        <?php endif; ?>


		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%" valign="top" rowspan="2">Ordem de Serviço:</td>
			<td valign="top">
				<?php
				$arquivo = new Arquivo( $arqid );
				?>
				<input type="hidden" name="arqid" id="arqid" value="<?php echo $arquivo->arqid; ?>"/>
				<input type="file" name="arquivo" id="arquivo"/>
				<br>
				<br>
				<?php
//				$arquivo = new Arquivo( $arqid );
                                    if ( $arquivo->arqid ){
                                        echo "<a href='?modulo=principal/cadOsMi&acao=A&req=downloadArquivo&arqid={$arquivo->arqid}'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>";
                                    }
				?>
			</td>
		</tr>

        <tr>
        <?php
            $dados =  $ordemServico->getDados();
            $estado = pegaEstadoOsMi($dados['docid']);
            if ($dados['tomid'] == 3  && ($estado == ESDID_OS_MI_VALIDACAO || $estado == ESDID_OS_MI_CONCLUIDA)) {
            $anexoOsMi = new AnexoOsMi();
            $anexoOsMi = $anexoOsMi->getAnexoExecucaoImplantacao($dados['osmid']);
            if (!empty($anexoOsMi) > 0) {
        ?>
        <tr>
            <td class="subtitulodireita" valign="top">Anexos: </td>
            <td>
                <?php foreach ($anexoOsMi as $anexo) { ?>
                    <?php $arquivo = new Arquivo($anexo['arqid']); ?>
                    <p>
                        <?php
                        if ($dados['tomid'] == 3 && $estado == ESDID_OS_MI_EXECUCAO || $estado == ESDID_OS_MI_CORRECAO) {
                            echo'<img style="float:left; margin-right:10px; cursor: pointer" src="/imagens/excluir.gif" alt="Excluir" title="Excluir" class="deleteArquivo" rel="' . $arquivo->arqid . '"/>';
                        }
                        echo "<a href='?modulo=principal/popupAceiteOS&acao=A&requisicao=downloadArquivo&arqid={$arquivo->arqid}'>" . $arquivo->arqnome . "." . $arquivo->arqextensao . " (" . $anexo['taonome'] . ")</a>";
                        ?>
                    </p>
                <?php } ?>
            </td>
        </tr>
    <?php
    }
    }
    ?>
        </tr>

		<tr bgcolor="#CCCCCC">
		   <td align="left">
		   	&nbsp;
		   </td>
		   <td colspan="2">
                        <?php if(empty($perfCallCenter)):
                                if($somenteLeitura == 'S'):?>
                                    <input type="button" name="botao" value="Salvar" onclick="validarFormOS();"/>
                            <?php endif;?>
                            <?php if (($estid == ESDID_OS_MI_CONCLUIDA || $estid ==  ESDID_OS_MI_REPROVADA || $estid ==  ESDID_OS_MI_CANCELADA) && isset($osmid)): ?>
                                <input type="button" name="botao" value="Nova OS" onclick="novaOS();"/>
                            <?php endif; ?>
                        <?php endif;?>
                                
				<input type="button" name="botao" value="Fechar" onclick="window.close();"/>
		   </td>
		</tr> 				
	</table>
</form>

<?php if ($dados['tomid'] == 3 && ($estado == ESDID_OS_MI_VALIDACAO || $estado == ESDID_OS_MI_CONCLUIDA)): ?>
    <?php
    $read = 'N';
    $anexoOs = new AnexoOsMi();
    $tipoOs = new TipoAnexoOsMi();
    $anexoOs = $anexoOs->getAnexoExecucaoImplantacao($dados['osmid']);
    ?>
    <form name="laudoOSimplantacao" id="laudoOSimplantacao" method="post" action=""  enctype="multipart/form-data" >
        <?php if ($estado == ESDID_OS_MI_EXECUCAO || $estado == ESDID_OS_MI_CORRECAO): ?>
    <?php $read = 'S'; ?>
    <input type="hidden" name="requisicao" id="requisicao" value="" />
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
            <tbody>
            <tr style="background: lightgray"><td colspan="2" width="100%" align="center"><label class="TituloTela" >Anexos da OS</label></td></tr>
            <tr>
                <td class="SubTituloDireita" style="vertical-align:top; width:25%">
                    <input type="button" id="addAnexo" value="Adicionar anexo"/>
                </td>
                <td style="vertical-align:top;">
                    <table id="anexosimplantacao">
                        <tr class="anexosimplantacao">
                            <td class="SubTituloDireita">
                                <select name="tipoarquivo[]" id="">
                                    <?php foreach ($tipoOs->getTiposPorTomid($tomid) as $tipo): ?>
                                        <option value="<?php echo  $tipo['taoid'] ?>"><?php echo  $tipo['taonome'] ?></option>
                                    <?php endforeach; ?>
                                </select>
                            </td>
                            <td>
                                <input type="file" name="arquivo[]" id="arquivo"/>
                            </td>
                        </tr>
                    </table>
                </td>
            </tr>

            <?php endif; ?>
            <?php
            if ($estado != ESDID_OS_MI_AGUARDANDO_ACEITE):
            $obra = new Obras($obrid);
            $end = $obra->getEnderecoObra($obra->obrid);

            $cronogramaPadrao = new Cronograma_PadraoMi();
            $cronograma = $cronogramaPadrao->pegaCronogramaPadrao($end['estuf'], $obra->tpoid);

            $icm = new Itens_Composicao_PadraoMi();
            $icplistForaEdificacao = $icm->pegaPorCpm($cronograma['cpmid'], 'F');

            $sql = "SELECT
                        umdid as codigo,
                        umdeesc as descricao
                    FROM obras2.unidademedida
                    ORDER BY umdeesc ASC
                   ";
            $unidadesDeMedida = $db->carregar($sql);
            ?>
            <tr><td colspan="2">
                    <div style="width:95%;margin-top:5px;padding:4px;text-align:center;font-weight:bold;margin: 0 auto;">Serviços Externos</div>
                    <table class="tabela listagem" style="text-align: center" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
                        <thead>
                        <tr>
                            <td><b>Item</b></td>
                            <td><b>Valor Unitário</b></td>
                            <td><b>Quantidade</b></td>
                            <td><b>Unidade de medida</b></td>
                            <td><b>Valor Total</b></td>
                        </tr>
                        </thead>
                        <tbody id="listaitensForaEdificacao">
                        <?php foreach ($icplistForaEdificacao as $icm): ?>
                            <?php
                            $qtdItens = new QtdItensComposicaoObraMi();
                            $qtdItem = $qtdItens->pegaPorObridEIcmid($obrid, $icm['icmid']);
                            ?>
                            <input type="hidden" value="<?php echo  $icm['icmid'] ?>" name="icmid[]"/>
                            <tr class='estruturapadraoForaEdificacao'>
                                <td>
                                    <?php
                                    $itens = new ItensComposicao();
                                    $db->monta_combo("itcid[]", $itens->listaCombo(), 'N', 'Selecione...', '', '', '', 200, 'S', '', '', $icm['itcid']);
                                    ?>
                                </td>
                                <td><?php
                                    $icmvalorconvertido = ($icm['icmvalor']) ? number_format($icm['icmvalor'], 2, ',', '.') : '';
                                    echo campo_texto('icmvalor[]', 'N', 'N', '', 9, 30, '####.###,##', '', 'right', '', 0, '', '', $icmvalorconvertido);
                                    ?></td>
                                <td><?php echo campo_texto('icmquantidade[]', 'N', $read, '', 9, 30, '#######,##', '', 'right', '', 0, '', 'jQuery(\'input[name=icmquantidade\[\]]\').change();', number_format($qtdItem['qioquantidade'], 2, ',', '.')); ?></td>
                                <td><?php echo $db->monta_combo("umdid[]", $unidadesDeMedida, 'N', 'Selecione...', '', '', '', 200, 'N', '', '', $icm['umdid']); ?></td>
                                <td><?php
                                    $icm['icmvalortotal'] = $qtdItem['qioquantidade'] * $icm['icmvalor'];
                                    $icmvalortotalconvertido = ($icm['icmvalortotal']) ? number_format($icm['icmvalortotal'], 2, ',', '.') : '';
                                    echo campo_texto('icmvalortotal[]', 'N', 'N', '', 9, 30, '####.###,##', '', 'right', '', 0, '', '', $icmvalortotalconvertido, '');
                                    ?>
                                </td>
                            </tr>
                        <?php endforeach; ?>
                        </tbody>
                        <tfoot>
                        <tr>
                            <td colspan="9" >
                                Total: <input type="text" disabled="disabled" id="vltot" />
                            </td>
                        </tr>
                        </tfoot>
                    </table>
                </td></tr>
            <?php if ($estado == ESDID_OS_MI_EXECUCAO || $estado == ESDID_OS_MI_CORRECAO): ?>
                <tr bgcolor="#C0C0C0">
                    <td></td>
                    <td>
                        <div style="float: left;">
                            <input type="button" value="Salvar" style="cursor: pointer" onclick="salvarArquivoImplantacao();" style="cursor: pointer;"/>
                            <input type="button" value="Fechar" style="cursor: pointer" onclick="window.close();">
                        </div>
                    </td>
                </tr>
            <?php endif; ?>
            </tbody>
        </table>
    <?php endif; ?>
    </form>


<?php endif; ?>


</body>
</html>

<?php
function checaOsImplantacao($obrid) {
    if($_GET['tomid'] == 1 || $_SESSION['tomid'] == 1){
        $osI = new OrdemServicoMI();
        $osI->carregarPorObridETomid($obrid, 3 );

        if(!$osI->osmid)
            return false;

        $esd = wf_pegarEstadoAtual($osI->docid);
        if ($esd['esdid'] == ESDID_OS_MI_CONCLUIDA) {
            return true;
        }
        return false;
    }
    return true;
}
?>