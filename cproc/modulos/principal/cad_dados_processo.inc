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
    
    if ($_REQUEST['requisicao'] == 'carregar_mantenedora') {
    	carregarMantenedoraPorMantida($_POST['iesid']);
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

    $titulo_modulo = "Dados do Processo - Cadastro";
    $modulo = "CPROC";
    monta_titulo($titulo_modulo, $modulo);

    $arMnuid = array();
    $db->cria_aba(ABA_CAD_PROCESSO, $url, '', $arMnuid);
    

// Processo --------------------------------------
if( $prcid )
    $Processo = new Processo( $prcid );  

// PERMISSOES
$habil = 'N';
$habilTramite = 'S';
// ver( $_SESSION,d);
$habil = Processo::permissoesFormularioDadosDoProcesso( $_SESSION['usucpf'] );

if ($habil != 'N'){
	$habil = retornaPermissao($prcid);
}

if( $prcid ){
	// busca documento
	$sql = "SELECT
					d.esdid
				FROM cproc.processo p
				INNER JOIN workflow.documento d ON p.docid = d.docid
				WHERE p.prcid = " . $prcid;
	$esdid = $db->carregar( $sql );
}

$pfls = arrayPerfil();

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

	/*
		Victor Martins Machado
		Função que carrega a Mantenedora quando a Mantida é selecionada
	*/
	function carregarMantenedoraPorMantida(){
        divCarregando();

        selectAllOptions(document.getElementById('iesid'));

        var iesid = document.getElementById('iesid').value;
		var mnt = document.getElementById('mntid');
        var string_array, array;
        
        jQuery.ajax({
            type: 'POST',
            url: 'cproc.php?modulo=principal/cad_dados_processo&acao=A',
            data: {requisicao: 'carregar_mantenedora', iesid: iesid},
            async: false,
            success: function(data) {
            	string_array = data;
            	array = string_array.split('|');
            	mnt.options[0] = null;
            	if( array[0] != '' ){
            		mnt.options[mnt.options.length] = new Option( array[1], array[0], false, false );
            	}
            	divCarregado();
            }
        });

		if (!document.getElementById('iesid').disabled){
	        if (document.getElementById('iesid').value != ""){
				$('#entid').attr("disabled", "disabled");
			} else {
				$('#entid').removeAttr("disabled");
			}
		}
	}

	function carregarMantenedora(){
		if (!document.getElementById('mntid').disabled){
			selectAllOptions(document.getElementById('mntid'));
	
			if (document.getElementById('mntid').value != ""){
				$('#iesid').attr("disabled", "disabled");
				$('#entid').attr("disabled", "disabled");
			} else {
				$('#iesid').removeAttr("disabled");
				$('#entid').removeAttr("disabled");
			}
		}
	}

	function carregarEntidade(){

		if (!document.getElementById('entid').disabled){			
			selectAllOptions(document.getElementById('entid'));
		
			if (document.getElementById('entid').value != ""){
				$('#mntid').attr("disabled", "disabled");
				$('#iesid').attr("disabled", "disabled");
			} else {
				$('#mntid').removeAttr("disabled");
				$('#iesid').removeAttr("disabled");
			}
		}
	}
		
    function salvarDadosProcesso(){
        
    	var erro;
        var campos = '';
		var numLen = document.getElementById('prcnumsicoc').value.length;
        
        console.log(1);
        selectAllOptions(document.getElementById('mntid'));
        selectAllOptions(document.getElementById('iesid'));
        selectAllOptions(document.getElementById('entid'));
        console.log(2);
        
		if (numLen < 17) {
			alert('Nº do Processo SIDOC deve conter 17 caractéres.');
			return false;
		}
        
		if (document.getElementById('mntid').value == '' && document.getElementById('iesid').value == '') {
			$('#entid').addClass('obrigatorio');
		} else {
			$('#entid').removeClass('obrigatorio');
		}
		
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

    function carregaMantenedora() {

		var ent = document.getElementById('entid');
		
		ent.options[0] = null;

		ent.options[ent.options.length] = new Option( 'Entidade A', '1', false, false );
		
    }
    
</script>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
	<tr>
		<td>
			<form method="POST" id="formulario" name="formulario" onsubmit="return validarProcesso();">
			    <?PHP if ($prcid) { ?>
			        <input type="hidden" id="requisicao" name="requisicao" value="alterar"/>
			        <input type="hidden" id="prcid" name="prcid" value="<?PHP echo $prcid; ?>"/>
			    <?PHP } else { ?>
			        <input type="hidden" id="requisicao" name="requisicao" value="cadastrar"/>
			    <?PHP } ?>
			
			    <table align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
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
			                    if(!empty($Processo) && $habilTramite == 'S'){
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
			                    echo campo_texto('prcnumsidoc', 'S', $habil, 'Nº do Processo SIDOC', '51', '20', '#####.######/####-##', '', '', '', '', 'id="prcnumsicoc"', '', '', '');
			                ?>
			            </td>
			        </tr>
			
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
							}

							$sql = "SELECT mntid AS codigo, TRIM(mntdsc) AS descricao FROM gestaodocumentos.mantenedoras WHERE 1=1 ";

							$arCampos = Array(Array('codigo' => 'mntid', 'descricao' => 'Código', 'numeric' => 1), Array('codigo' => 'mntdsc', 'descricao' => 'Mantenedora'));
							combo_popup('mntid', $sql, 'Mantenedora', '500x400', '1', '', '', $habil, '', '', '2', '450', null, null, false, $arCampos, $rmantenedora, true, false, 'carregarMantenedora()', false, null, null);
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
			                    }
			
			                    $sql = "SELECT iesid AS codigo, TRIM(iesdsc) AS descricao FROM gestaodocumentos.instituicaoensino ORDER BY iesdsc";
			                    $arCampos = Array(Array('codigo' => 'iesid', 'descricao' => 'Código', 'numeric' => 1), Array('codigo' => 'iesdsc', 'descricao' => 'IES'), Array('codigo' => 'iessigla', 'descricao' => 'Sigla'));
			                    combo_popup('iesid', $sql, 'IES', '460x400', '1', '', '', $habil, '', '', '2', '450', null, null, false, $arCampos, $rmantida, true, false, 'carregarMantenedoraPorMantida()', false, null, null);
			                ?>
			            </td>
			        </tr>
			        <tr>
			            <td class="subtituloDireita">Entidade:</td>
			            <td>
			                <?PHP
				                if ($entid) {
				                	$sql = "SELECT entid AS codigo, TRIM(entdsc) as descricao FROM cproc.entidade WHERE entid = {$entid}";
				                	$rentidade = $db->carregar($sql);
				                }
			                
			                	$sql = "SELECT entid AS codigo, TRIM(entdsc) as descricao FROM cproc.entidade";	
			                	
			                	$arCampos = Array(Array('codigo' => 'entid', 'descricao' => 'Código', 'numeric' => 1), Array('codigo' => 'entdsc', 'descricao' => 'Entidade'));
			                	combo_popup('entid', $sql, 'Entidade', '500x400', '1', '', '', $habil, '', '', '2', '450', null, null, false, $arCampos, $rentidade, true, false, 'carregarEntidade()', false, null, null);
			                    //echo campo_texto('entid', 'N', $habil, 'Entidade', '51', '50', '', '', '', '', '', 'id="entid"', '', '', '');
			                ?>
			            </td>
			        </tr>
			        
			        <!-- TRECHO ADICIONADO AO CABECALHO -->
			        <!-- Responsáveis -->
			            <?php responsaveisProcesso($prcid)?>
			        <!-- / -->
			                
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
			                    echo campo_texto('prclocaloferta', 'N', $habil, 'Local de Oferta', '51', '50', '', '', '', '', '', 'id="prclocaloferta" class="required"', '', '', '');
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
			                    echo campo_data2('prcdtentradaproc', 'S', $habil, 'Data de Entrada do Processo', 'S', '', '', '', '', '', 'prcdtentradaproc');
			                ?>
			            </td>
			        </tr>
			        <!-- datas extras -->
			        <tr>
			            <td class="subtituloDireita" colspan="2">Prazo (análise):</td>
			            <td>
			                <?php 
			                if( $Processo ){
			
			                    $dadosCoordenadorResponsavel = $Processo->buscaDadosCoordenadorResponsavel();
			                    
			                    $apresentacao = '';
			
			                    if( $dadosCoordenadorResponsavel[0]['crpprazodefinido'] )
			                        $apresentacao .= $dadosCoordenadorResponsavel[0]['crpprazodefinido'].' dias ';
			
			                    if( $dadosCoordenadorResponsavel[0]['trpdtfinalprazo'] )
			                        $apresentacao .= '- ('.$dadosCoordenadorResponsavel[0]['trpdtfinalprazo'].')';
			
			                    echo $apresentacao;
			                }
			                ?>
			            </td>
			        </tr>
			        <tr>
			            <td class="subtituloDireita" colspan="2">Prazo Execução:</td>
			            <td>
			                <?php
			                    if( $Processo )
			                        echo $Processo->pegaDataExecucao();
			                ?>
			            </td>
			        </tr>
			        <tr>
			            <td class="subtituloDireita" colspan="2">Prazo Manifestação:</td>
			            <td>
			                <?php
			                    if( $Processo )
			                        echo $Processo->pegaDataManifestacao();
			                ?>
			            </td>
			        </tr>
			        <!-- datas extras -->
			
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
			                <input type="radio" name="prcstatusead" value="S" class="normal obrigatorio" title="Modalidade EAD" style="margin-top: -1px;" <?PHP echo(($prcstatusead == "f") ? "checked" : ""); ?>> Sim
			                <input type="radio" name="prcstatusead" value="N" class="normal obrigatorio" title="Modalidade EAD" style="margin-top: -1px;" <?PHP echo(($prcstatusead == "t") ? "checked" : ""); ?>> Não
			                &nbsp;&nbsp;
			                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório." class="normal" style="margin-top: -1px;">
			            </td>
			        </tr>
			        
			        <tr>
			            <td class="subtituloDireita" colspan="2">Supervisão Especial:</td>
			            <td>
			                <input type="radio" name="prcstatussupesp" value="t" class="normal obrigatorio" title="Supervisão Especial" style="margin-top: -1px;" <?PHP echo(($prcstatussupesp == "f") ? "checked" : ""); ?>> Sim
			                <input type="radio" name="prcstatussupesp" value="f" class="normal obrigatorio" title="Supervisão Especial" style="margin-top: -1px;" <?PHP echo(($prcstatussupesp == "t") ? "checked" : ""); ?>> Não
			                &nbsp;&nbsp;
			                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório." class="normal" style="margin-top: -1px;">
			            </td>
			        </tr>
			        <tr>
			            <td class="subtituloDireita" colspan="2">Assunto:</td>
			            <td>
			                <?PHP 
			                    echo campo_textarea('prcassunto', 'N', $habil, 'Assunto', '100', '10', '2000', '','','','','','', '44%'); 
			                ?>
			            </td>
			        </tr>
			        <tr>
			            <td class="SubTituloCentro" colspan="3">
			            	<?php 
			            		if ($habil == 'S'){
			            	?>
			                <input type="button" name="salvarProcesso" id="salvarProcesso" value="Salvar" onclick="salvarDadosProcesso();"/>
			                <?php } ?>
			            </td>
			        </tr>
			
			        <!-- PROCESSOS ANEXADOS -->
			        <?php if( $_GET['prcid'] ){ ?>
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
			                            '<a title=\"Desanexar Processo\" onclick=\"excluirAnexacaoProcesso('|| pa.praid ||');\"><img src=\"/imagens/excluir.gif\" /></a>' as acao,
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
			        <tr>
			            <td colspan="2">       	
			            	<?php
			            		if (in_array(PERFIL_SUPER_USUARIO, $pfls) || in_array(PERFIL_APOIO_CPROC, $pfls)) {
									if ($esdid[0]['esdid'] == CPROC_EMCADASTRAMENTO_ESDID || 
										$esdid[0]['esdid'] == CPROC_ATUALIZACAODOPROCEDIMENTO_ESDID || 
										$esdid[0]['esdid'] == CPROC_IMPRESSAO_ASSINATURA_E_NUMERACAO_DO_DOCUMENTO_ESDID || 
										$esdid[0]['esdid'] == CPROC_AGUARDARMANIFESTACAOINTERESSADO_ESDID || 
										$esdid[0]['esdid'] == CPROC_ARQUIVARPROCEDIMENTO_ESDID) { 
							?>
			            	<input type="button" value="Anexar processo" onclick="abreJanelaProcessosVinculados();">
			            	<?php 
			            		}
									} 
							?>
			            </td>
			        </tr>
			        <?php } ?>
			
			    </table>
			</form>		
		</td>
		<td valign="top">
            <?php
	            if(!empty($Processo) && $habilTramite == 'S'){
	            	WorkflowCproc::aplicaWorkflowNaPagina( $Processo );
	            }
            ?>
		</td>
	</tr>




<script>                
    function abreJanelaProcessosVinculados(){

        windowOpen('?modulo=principal/vincular_processo&acao=A&prcid=<?=$_GET['prcid']?>','blank','height=600,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    function excluirAnexacaoProcesso( praid ){

        if( confirm("Deseja desanexar Processo?") ){
            window.location.href = window.location.href + '&excluirpraid='+praid;
        }
    }
</script>