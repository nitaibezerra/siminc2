<?PHP 

// dependências --------------------------------------------------------------------------------------------------
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . "cproc/classes/ModeloCproc.class.inc";
include_once APPRAIZ . "cproc/classes/Processo.class.inc";
include_once APPRAIZ . "cproc/classes/WorkflowCproc.class.inc";
// --------------------------------------------------------------------------------------------------
    if ($_REQUEST['requisicao'] == 'cadastrar') {
        salvarProcesso($_POST);
        exit();
    }

    if ($_REQUEST['requisicao'] == 'alterar') {
        alterarProcesso($_POST);
        exit();
    }

    if ($_REQUEST['requisicao'] == 'carregar_municipio') {
        carregarMunicipiosPorUF($_POST);
        exit();
    }

    if (empty($_SESSION['cproc']['prcid']) && isset($_REQUEST['prcid'])) {
        $_SESSION['cproc']['prcid'] = $_REQUEST['prcid'];
    }

    if ($_SESSION['cproc']['prcid']) {
         $processo = exibirProcesso($_SESSION['cproc']['prcid']);
         
        extract($processo);
    }

    include APPRAIZ . "includes/cabecalho.inc";

    $titulo_modulo = "Resumo";
    $modulo = "CPROC";
    monta_titulo($titulo_modulo, $modulo);

    $arMnuid = array();
    $db->cria_aba(ABA_CAD_PROCESSO, $url, '', $arMnuid);
    

// Processo --------------------------------------
if( $prcid )
    $Processo = new Processo( $prcid );  

// PERMISSOES
$habil = 'N';
// ver( $_SESSION,d);
//$habil = Processo::permissoesFormularioDadosDoProcesso( $_SESSION['usucpf'] );



// excluir anexacao
if( $_GET['excluirpraid'] ){
    $sql = " 
        SELECT COUNT(*) as total
        FROM cproc.processoanexado
        WHERE praid = {$_GET['excluirpraid']}
    ";
    $resultado = $db->carregar( $sql );
    if( $resultado[0]['total'] > 0 ){
        $sql = "
            DELETE FROM cproc.processoanexado
            WHERE praid = {$_GET['excluirpraid']}
        ";
        $db->executar($sql);
        if($db->commit()){
            echo "<script>
                alert('Anexação removida com sucesso!');
                window.history.back();
            </script>";
        }else{
            echo "<script>
                alert('Erro ao remover anexação!');
                window.history.back();
            </script>";
        }
        exit;
    }else{
        echo "<script>
            window.history.back();
        </script>";
        exit;
    }
}

?>

<br>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script language="javascript" type="text/javascript">
    function carregarMunicipiosPorUF(estuf) {
        divCarregando();
        jQuery.ajax({
            type: 'POST',
            url: 'cproc.php?modulo=principal/cad_dados_processo&acao=A',
            data: {requisicao: 'carregar_municipio', estuf: estuf},
            async: false,
            success: function(data) {
                jQuery('#divMunicipio').html(data);
                divCarregado();
            }
        });
    }

    function salvarDadosProcesso(){
        
        var erro;
        var campos = '';
        
        console.log(1);
        selectAllOptions(document.getElementById('mntid'));
        selectAllOptions(document.getElementById('iesid'));
        console.log(2);

        jQuery('#mntid').attr('title', 'Mantenedora');
        console.log(3);
        
        jQuery.each(jQuery(".obrigatorio"), function(i, v){
            if( jQuery(this).attr('type') != 'radio' ){
                if ( jQuery(v).val() == '' ){
                    erro = 1;
                    campos += '- ' + jQuery(this).attr('title') + " \n";
                }
            }else{
                var name  = jQuery(this).attr('name');
                var value = jQuery(this).attr('value');
                var radio_box = jQuery('input:radio[name='+name+']:checked');

                if(!radio_box.val()){
                    erro = 1;
                    if(value == 'S'){
                        campos += '- ' + jQuery(this).attr('title') + " \n";
                    }
                }
            }
        });

        if( erro > 0 ){
            alert('Existem "campos obrigatorios" vazios! São eles: \n'+campos);
            return false;
        }

        if(!erro){
            jQuery('#formulario').submit();
        }
    }
    
</script>

<form method="POST" id="formulario" name="formulario" onsubmit="return validarProcesso();">
    <?PHP if ($prcid) { ?>
        <input type="hidden" id="requisicao" name="requisicao" value="alterar"/>
        <input type="hidden" id="prcid" name="prcid" value="<?PHP echo $prcid; ?>"/>
    <?PHP } else { ?>
        <input type="hidden" id="requisicao" name="requisicao" value="cadastrar"/>
    <?PHP } ?>

    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
        <tr>
            <td class="subtituloDireita" width="20%" colspan="2">Nº Controle SIMEC:</td>
            <td>
                <?php
                    echo campo_texto('prcid', 'S', 'N', 'Nº Controle ' SIGLA_SISTEMA, '51', '25', '', '', '', '', '', 'id="prcid"', '', '', '');
                ?>
            </td>

            <!-- WORKFLOW -->
            <td style="width:300px !important;" rowspan="15" valign="top" align="right">
                <?php
                    if(!empty($Processo)){
                    //WorkflowCproc::aplicaWorkflowNaPagina( $Processo );
                    }
                ?>
            </td>

        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Tipo de Processo:</td>
            <td>
                <?PHP
                    $sql = "
                        SELECT  tprid AS codigo,
                                tprdsc AS descricao
                        FROM cproc.tipoprocesso
                        ORDER BY tprdsc
                    ";
                    $db->monta_combo('tprid', $sql, $habil, 'Selecione...', '', '', '', '455', 'S', '', '','','Tipo de Processo');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Nº do Processo SIDOC:</td>
            <td>
                <?php
                    echo campo_texto('prcnumsidoc', 'S', $habil, 'Nº do Processo SIDOC', '51', '17', '#####.######/####-##', '', '', '', '', 'id="prcnumsicoc"', '', '', '');
                ?>
            </td>
        </tr>

        <!-- TRECHO ADICIONADO AO CABECALHO -->
        <!-- Responsáveis -->
            <?php responsaveisProcesso($prcid)?>
        <!-- / -->

        <tr>
            <td class="subtituloDireita" rowspan="4" width="20%">Interessado:</td>
        </tr>
        <tr>
            <td class="subtituloDireita" width="12%">Mantenedora:</td>
            <td>
                <?PHP
                    if ($mntid) {
                        $sql = "SELECT mntid AS codigo, TRIM(mntdsc) AS descricao FROM gestaodocumentos.mantenedoras WHERE mntid = {$mntid}";
                        $rmantenedora = $db->carregar($sql);
                        echo $rmantenedora[0]['descricao'];
                    }
                    
                    

                    //$sql = "SELECT mntid AS codigo, TRIM(mntdsc) AS descricao FROM gestaodocumentos.mantenedoras";

                    //$arCampos = Array(Array('codigo' => 'mntdsc', 'descricao' => 'Mantenedora'), Array('codigo' => 'mntcnpj', 'descricao' => 'CNPJ'));
                    //combo_popup('mntid', $sql, 'Mantenedora', '100x100', '1', '', '', $habil, '', '', '1', '450', null, null, false, $arCampos, $rmantenedora, true, false, '', false, null, null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Mantida:</td>
            <td>
                <?PHP
                    if ($iesid) {
                        $sql = "SELECT iesid AS codigo, TRIM(iesdsc) AS descricao FROM gestaodocumentos.instituicaoensino WHERE iesid = {$iesid} ORDER BY iesdsc";
                        $rmantida = $db->carregar($sql);
                        echo $rmantida[0]['descricao'];
                    }

                    //$sql = "SELECT iesid AS codigo, TRIM(iesdsc) AS descricao FROM gestaodocumentos.instituicaoensino ORDER BY iesdsc";
                    //$arCampos = Array(Array('codigo' => 'iesdsc', 'descricao' => 'IES'));
                    //combo_popup('iesid', $sql, 'IES', '460x400', '1', '', '', $habil, '', '', '2', '450', null, null, false, $arCampos, $rmantida, true, false, '', false, null, null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Entidade:</td>
            <td>
                <?PHP
                    echo campo_texto('prcentidade', 'N', $habil, 'Entidade', '51', '50', '', '', '', '', '', 'id="prcentidade"', '', '', '');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Curso:</td>
            <td>
                <?PHP
                    echo campo_texto('prccurso', 'N', $habil, 'Curso', '51', '50', '', '', '', '', '', 'id="prccurso"', '', '', '');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Local de Oferta:</td>
            <td>
                <?PHP
                    echo campo_texto('prclocaloferta', 'N', $habil, 'Local de Oferta', '51', '50', '', '', '', '', '', 'id="prclocaloferta"', '', '', '');
                ?>
            </td>
        </tr>
        <?php /*<tr>
            <td class="subtituloDireita" colspan="2">Polo:</td>
            <td><?PHP echo campo_texto('prcpoloferta', 'N', $habil, 'Polo', '51', '100', '', '', '', '', '', 'id="prcpoloferta"', '', '', ''); ?></td>
        </tr> */ ?>
        <tr>
            <td class="subtituloDireita" colspan="2">Data de Entrada do Processo:</td>
            <td>
                <?PHP
                    echo campo_data2('prcdtentradaproc', 'N', $habil, 'Data de Entrada do Processo', 'S', '', '', '', '', '', 'prcdtentradaproc');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">UF:</td>
            <td>
                <?PHP
                    $sql = "SELECT estuf AS codigo, estuf AS descricao FROM territorios.estado ORDER BY estuf";
                    $db->monta_combo('estuf', $sql, $habil, 'Selecione...', 'carregarMunicipiosPorUF', '', '', '151', 'N', 'estuf', '','','UF');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Município:</td>
            <td id="divMunicipio">
                <?PHP
                    $sql = "SELECT muncod AS codigo, mundescricao AS descricao FROM territorios.municipio WHERE estuf = '{$estuf}' ORDER BY mundescricao";
                    $db->monta_combo('muncod', $sql, $habil, 'Selecione...', '', '', '', '455', 'N', '', '','','Município');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Modalidade EAD:</td>
            <td>
                <input disabled="disabled" type="radio" name="prcstatusead" value="S" class="normal obrigatorio" title="Modalidade EAD" style="margin-top: -1px;" <?PHP echo(($prcstatusead == "f") ? "checked" : ""); ?>> Sim
                <input disabled="disabled" type="radio" name="prcstatusead" value="N" class="normal obrigatorio" title="Modalidade EAD" style="margin-top: -1px;" <?PHP echo(($prcstatusead == "t") ? "checked" : ""); ?>> Não
            </td>
        </tr>
        
        <tr>
            <td class="subtituloDireita" colspan="2">Supervisão Especial:</td>
            <td>
                <input disabled="disabled" type="radio" name="prcstatussupesp" value="t" class="normal obrigatorio" title="Supervisão Especial" style="margin-top: -1px;" <?PHP echo(($prcstatussupesp == "f") ? "checked" : ""); ?>> Sim
                <input disabled="disabled" type="radio" name="prcstatussupesp" value="f" class="normal obrigatorio" title="Supervisão Especial" style="margin-top: -1px;" <?PHP echo(($prcstatussupesp == "t") ? "checked" : ""); ?>> Não
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Assunto:</td>
            <td>
                <?PHP 
                    echo campo_textarea('prcassunto', 'N', $habil, 'Assunto', '67', '4', '2000', '','','','','',''); 
                ?>
            </td>
        </tr>
        
        <tr>
            <td class="subtituloDireita" rowspan="8" width="20%">Detalhamento:</td>
        </tr>
        <tr>
            <td class="subtituloDireita" width="12%">Minutas:</td>
            <td>
               <?PHP
                $sql = "SELECT count(mntid)
						FROM cproc.minuta
						where prcid = ".$_SESSION['cproc']['prcid']; 
                $total = $db->pegaUm($sql);

                if($total>0){
                	echo '&nbsp;<img border="0" src="../imagens/valida1.gif" title="Existe Minutas." class="normal">';
                }else{
                	echo '&nbsp;<img border="0" src="../imagens/valida6.gif" title="Não existe Minutas." class="normal">';
                }
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" width="12%">Documentos:</td>
            <td>
               <?PHP
                $sql = "SELECT count(docid)
						FROM cproc.documento
						where prcid = ".$_SESSION['cproc']['prcid']; 
                $total = $db->pegaUm($sql);

                if($total>0){
                	echo '&nbsp;<img border="0" src="../imagens/valida1.gif" title="Existe Documentos." class="normal">';
                }else{
                	echo '&nbsp;<img border="0" src="../imagens/valida6.gif" title="Não existe Documentos." class="normal">';
                }
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" width="12%">Medida Cautelar:</td>
            <td>
                <?PHP
                $sql = "SELECT count(m.vmcid)
						FROM cproc.medidacautelar m
						inner join cproc.vigenciamedcautelar v on v.vmcid = m.vmcid 
						where v.vmcvigencia = true
						and prcid = ".$_SESSION['cproc']['prcid'];   
                $total = $db->pegaUm($sql);

                if($total>0){
                	echo '&nbsp;<img border="0" src="../imagens/valida1.gif" title="Existe Medida Cautelar." class="normal">';
                }else{
                	echo '&nbsp;<img border="0" src="../imagens/valida6.gif" title="Não existe Medida Cautelar." class="normal">';
                }
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" width="12%">Medida Saneadora:</td>
            <td>
                <?PHP
                $sql = "SELECT count(m.vmsid)
						FROM cproc.medidasaneadora m
						inner join cproc.vigenciamedsaneadora v on v.vmsid = m.vmsid 
						where v.vmsvigencia = true
						and prcid = ".$_SESSION['cproc']['prcid'];   
                $total = $db->pegaUm($sql);

                if($total>0){
                	echo '&nbsp;<img border="0" src="../imagens/valida1.gif" title="Existe Medida Saneadora." class="normal">';
                }else{
                	echo '&nbsp;<img border="0" src="../imagens/valida6.gif" title="Não existe Medida Saneadora." class="normal">';
                }
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" width="12%">Penalidade aplicada:</td>
            <td>
                <?PHP
                $sql = "SELECT count(m.vpaid)
						FROM cproc.penalidadeaplicada m
						inner join cproc.vigenciapenaplicada v on v.vpaid = m.vpaid 
						where v.vpavigencia = true
						and prcid = ".$_SESSION['cproc']['prcid'];   
                $total = $db->pegaUm($sql);

                if($total>0){
                	echo '&nbsp;<img border="0" src="../imagens/valida1.gif" title="Existe Penalidade aplicada." class="normal">';
                }else{
                	echo '&nbsp;<img border="0" src="../imagens/valida6.gif" title="Não existe Penalidade aplicada." class="normal">';
                }
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" width="12%">Ministério Público:</td>
            <td>
               <?PHP
                $sql = "SELECT count(m.pmpid)
						FROM cproc.ministeriopublico m
						inner join cproc.pendenciaministpublico v on v.pmpid = m.pmpid 
						where v.pmpvigencia = true
						and prcid = ".$_SESSION['cproc']['prcid'];   
                $total = $db->pegaUm($sql);

                if($total>0){
                	echo '&nbsp;<img border="0" src="../imagens/valida1.gif" title="Existe Ministério Público." class="normal">';
                }else{
                	echo '&nbsp;<img border="0" src="../imagens/valida6.gif" title="Não existe Ministério Público." class="normal">';
                }
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" width="12%">Verificação in Loco:</td>
            <td>
               <?PHP
                $sql = "SELECT count(m.vrlid)
						FROM cproc.verificacaoinloco m
						where prcid = ".$_SESSION['cproc']['prcid']; 
                $total = $db->pegaUm($sql);

                if($total>0){
                	echo '&nbsp;<img border="0" src="../imagens/valida1.gif" title="Existe Verificação in Loco." class="normal">';
                }else{
                	echo '&nbsp;<img border="0" src="../imagens/valida6.gif" title="Não existe Verificação in Loco." class="normal">';
                }
                ?>
            </td>
        </tr>

        <!-- PROCESSOS ANEXADOS -->
        <tr>
            <td class="SubTituloEsquerda" colspan="4">Processo(s) Anexado(s)</td>
        </tr>
        <tr>
            <td colspan="4">
                <table id="tabela_vincprocessos" width="100%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
                <tr>
                    <td valign="top" align="center" class="title" style="width:80px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ações</strong></td>
                    <td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Nº do processo</strong></td>
                    <td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Tipo</strong></td>
                    <td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data Entrada</strong></td>
                    <td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Situação</strong></td>
                </tr>
                <?php
                $sql = "
                        SELECT
                            --'<a onclick=\"excluirAnexacaoProcesso('|| pa.praid ||');\"><img src=\"/imagens/excluir.gif\" /></a>' as acao,
                            '' as acao,
                            prc.prcnumsidoc as numero,
                            tp.tprdsc as tprdsc,
                            TO_CHAR(pa.pradtanexacao,'DD/MM/YYYY') as pradtanexacao,
                            ed.esddsc as esddsc
                        FROM cproc.processoanexado pa
                        INNER JOIN cproc.processo prc ON pa.pro_prcid = prc.prcid
                        INNER JOIN cproc.tipoprocesso tp ON tp.tprid = prc.tprid
                        INNER JOIN workflow.documento d ON d.docid = prc.docid
                        INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
                        WHERE 
                            pa.prcid = ".$_SESSION['cproc']['prcid'];
                // ver($sql,d);
                $pvinculados = $db->carregar($sql);
                
                if($pvinculados[0]) {
                    foreach($pvinculados as $in) {
                        echo "<tr>";
                        echo "<td><center>".$in['acao']."</center></td>";
                        echo "<td>".$in['numero']."</td>";
                        echo "<td>".$in['tprdsc']."</td>";
                        echo "<td>".$in['pradtanexacao']."</td>";
                        echo "<td>".$in['esddsc']."</td>";
                        echo "</tr>";
                    }
                }
                ?>
                </table>
            </td>
        </tr>
        

    </table>
</form>

