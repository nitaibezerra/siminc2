<?php

// empreendimento || obra || orgao
if (!empty($_GET['obrid']) && $_SESSION['obras2']['obrid'] != $_GET['obrid']) {
    $_SESSION['obras2']['obrid'] = $_GET['obrid'];
    $objObr                      = new Obras($_SESSION['obras2']['obrid']);
    $_SESSION['obras2']['empid'] = $objObr->empid;
    if(empty($_SESSION['obras2']['orgid'])){
        $objEmp = new Empreendimento($_SESSION['obras2']['empid']);
        $_SESSION['obras2']['orgid'] = $objEmp->orgid;
        unset($objEmp);
    }
        unset($objObr);
}

if($_REQUEST['requisicao'] == 'ajaxCamposArquivo'){
    carregaCampoArquivo();
    die();
}

if($_REQUEST['requisicao'] == 'download'){
    download();
    die();
}

if($_REQUEST['requisicao'] == 'excluirArquivo'){
    excluirArquivo();
    die();
}

verificaSessao( 'obra' );

$restricao = new Restricao( $_REQUEST['rstid'] );

switch ( $_POST['requisicao'] ){
	case 'salvar':
            
            if(possui_perfil(array(PFLCOD_SUPERVISOR_UNIDADE)) && !($_POST['only_file'] == 'true' || $_POST['only_file'] == true) ){
                $str = "<script>
                                    alert('A Restrição/Inconformidade não pode ser alterada pois você não tem as permissões necessárias.'); 
                                    //window.opener.location.reload(); 
                                    document.location.href = document.location.href;
                             </script>";

                        die($str);
            }
            
            if($_POST['only_file'] == 'false' || $_POST['only_file'] == false ){
                
                if(!empty($_REQUEST['rstid']) && !possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC) )){
                    $esdid = $restricao->getDadosTramitacaoRestricaoInconformidade($restricao->docid, 'esd.esdid', '');
                    $esdid = $esdid ['esdid'];
                    if($esdid == ESDID_SUPERADA){
                        $str = "<script>
                                    alert('A Restrição/Inconformidade não pode ser alterada pois se encontra Superada.'); 
                                    //window.opener.location.reload(); 
                                    document.location.href = document.location.href;
                             </script>";

                        die($str);
                    }
                }
                
                if($_POST['rstflressalva'] == 'S' && trim($_POST['rstobsressalva']) == ''){
                    $str = "<script>
                                    alert('Restrição/Inconformidade com Ressalva, preencha o campo de observação.'); 
                                    document.location.href = document.location.href;
                             </script>";

                    die($str);
                }
            
		$arDado          = $_POST;
		$arDado['rstid'] = (empty($_REQUEST['rstid']) ? null : $_REQUEST['rstid']);

                $rstid = $restricao->salvaRestricaoInconformidade($arDado);

                $tipoRestricao = new TipoRestricao($arDado['tprid']);

                if(is_array($rstid)){
                    $str = "<script>
                                    alert('Ocorreu um erro inesperado e Restrição/Inconformidade não pode ser salva. ".$rstid['msg']." '); 
                                    document.location.href = document.location.href;
                             </script>";

                    die($str);
                }
                
                $arDado['rstid'] = $rstid;
                
		$restricao->atualizaDocidNullRetricao();

                // Modificado para atender a solicitação da Fernanda Ribeiro do FNDE - 10/12/2013
                if(isset($arDado['rstid'])){
                    executaRegistroAtividade($arDado);
                }
                
		$db->commit();	
                
                if(!empty($_FILES["arquivo"]) && $tipoRestricao->liberaanexo == 't'){
                    $resp = $restricao->gravaArquivosRestricaoInconformidade($restricao->rstid, $arDado);
                    $msg = $resp['msg'];
                }
                
            }else{
                
                if(!empty($_FILES["arquivo"])){
                    $resp = $restricao->gravaArquivosRestricaoInconformidade($restricao->rstid, $arDado);
                    $msg = $resp['msg'];
                }
            }

            $s = new SolicitacaoDesembolso();
            $s->verificaTramiteSolicitacoes($_SESSION['obras2']['obrid']);

            $str = "<script>
                        alert('Operação realizada com sucesso!".$msg."'); 
                        //window.opener.location.reload(); 
                        document.location.href = document.location.href;
                    </script>";

            die($str);
}

function executaRegistroAtividade($arDado){
    global $db;    
    $regAtividade = new RegistroAtividade();    
    
    $arDadoRA['obrid']              = $_SESSION['obras2']['obrid'];
    $arDadoRA['usucpf']             = $_SESSION['usucpf'];
    $arDadoRA['empid']              = $_SESSION['obras2']['empid'];
    $arDadoRA['rgadscsimplificada'] = 'Restrição/Inconformidade ('.$arDado['rstid'].') alterada.';
    $arDadoRA['rgadsccompleta']     = 'O registro de Restrição/Inconformidade de ID '.$arDado['rstid'].' foi alterado pelo GESTOR MEC com o CPF '.$_SESSION['usucpf'].'.';
    $arDadoRA['rgadtinclusao']      = date('Y-m-d');
    $arDadoRA['rgastatus']          = 'A';
    $arDadoRA['rgaautomatica']      = TRUE;
    
    $arCamposNuloRA   = array();
    $arCamposNuloRA[] = 'arqid'; 	
    $arCamposNuloRA[] = 'rgaid_1'; 	

    $regAtividade->popularDadosObjeto( $arDadoRA )
                 ->salvar(true, true, $arCamposNuloRA);
    $db->commit();
    
    return true;
}

$dados_restricao = $restricao->getDados();
//ver($dados_restricao);
extract( $dados_restricao );
$rstdtsuperacao = formata_data($rstdtsuperacao);

$faseRestricao = new FaseRestricao();
$tipoRestricao = new TipoRestricao($tprid);

/*
 * Regra complexa ter cuidado ao mexer
 */
if ( (  
        (!$usucpfsuperacao && !($qtsid || $rsqid) && $usucpf == $_SESSION['usucpf']) || 
        ( !$usucpfsuperacao && !($qtsid || $rsqid) && possui_perfil(array(PFLCOD_SUPER_USUARIO)) )
     ) || empty($rstid) ){
    $somenteLeituraConteudo  = 'S';
    $habilitadoConteudo      = true;
} else{
	$somenteLeituraConteudo  = 'N';
	$habilitadoConteudo 	 = false;
}
if((empty($usucpfsuperacao)) && (  possui_perfil( array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_GESTOR_UNIDADE)) && $usucpf == $_SESSION['usucpf'] )) {
	$somenteLeituraSuperacao = 'S';
	$habilitadoSuperacao 	 = true;
} else if ( possui_perfil( array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO) ) ){
	$somenteLeituraSuperacao = 'S';
	$habilitadoSuperacao 	 = true;
}else{
	$somenteLeituraSuperacao = 'N';
	$habilitadoSuperacao 	 = false;
}

if(possui_perfil( array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_GESTOR_UNIDADE, PFLCOD_EMPRESA_MI_GESTOR, PFLCOD_EMPRESA_MI_FISCAL))){
    $somenteLeituraConteudo  = 'N';
    $habilitadoConteudo      = false;
    $somenteLeituraSuperacao = 'N';
    $habilitadoSuperacao     = false;
    $habilitadoBotao         = false;
}

if(possui_perfil( array(PFLCOD_GESTOR_UNIDADE))){
    $somenteLeituraConteudo  = 'N';
    $habilitadoConteudo      = false;
    $somenteLeituraSuperacao = 'N';
    $habilitadoSuperacao     = false;
    $habilitadoBotao         = true;
}

//VER($somenteLeituraConteudo,$habilitadoConteudo,$somenteLeituraSuperacao,$habilitadoSuperacao);

// Modificado para atender a solicitação da Fernanda Ribeiro do FNDE - 10/12/2013
if ( possui_perfil( array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO) ) ){
    $somenteLeituraConteudo  = 'S';
    $habilitadoConteudo      = true;
    $somenteLeituraSuperacao = 'S';
    $habilitadoSuperacao     = true;
}
//VER($somenteLeituraConteudo,$habilitadoConteudo,$somenteLeituraSuperacao,$habilitadoSuperacao);

$habilitadoBotao = ( !$habilitadoSuperacao && !$habilitadoConteudo ) ? false : true;

/**
 * Verifica se a restrição já está superada, desabilitando a sua edição.
 */
$rest  = new Restricao();
$esdid = $rest->getDadosTramitacaoRestricaoInconformidade($docid, 'esd.esdid', '');
$esdid = $esdid ['esdid'];

if($esdid == ESDID_SUPERADA || (possui_perfil(PFLCOD_CALL_CENTER)) ){
    $somenteLeituraConteudo  = 'N';
    $habilitadoConteudo      = false;
    $somenteLeituraSuperacao = 'N';
    $habilitadoSuperacao     = false;
    $habilitadoBotao         = false;
}

?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<!--<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>-->
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script language="JavaScript">
    
    function fechaJanela(){
        var url_opener  = window.opener.location.href;
        var verificador = url_opener.indexOf("listaRestricao&acao=L");
        if(verificador == -1){
            window.opener.location.reload(); 
        }
        window.close();
    }
    
    $(document).ready(function(){
        
        // Tratamento colocado para retirar o # da URL, pois o mesmo estava impedindo o reload da página após as ações de tramitação e submit do form na sequencia.
        var v = String( document.location.href ).search('#');
        if(v != -1){ document.location.href = String( document.location.href ).replace( "#/", "" ).replace( "#", "" ); }
        
        $('#addArquivo').click(function(){
            var url   = 'obras2.php?modulo=principal/cadRestricao&acao=O&requisicao=ajaxCamposArquivo';
            var htm   = '';
            $.ajax({
                  url: url,
                  cache: false
                })
                .done(function( html ) {
                    htm = htm+' <tr> '+'<td colspan="3">'+ html + '</td>'+ ' </tr>';
                    $( "#camposDinamicosArquivos" ).append( htm );
                });
        });
        
        
        $('.download').click(function(){
            var id = $(this).attr('id');
            $('#requisicao').val('download');
            $('#arqid').val(id);
            $('#formulario').submit();
        });
        
        $('.excluir').click(function(){
            var id = $(this).attr('id');
            $('#requisicao').val('excluirArquivo');
            $('#arqid').val(id);
            $('#formulario').submit();
        });
        
    });
    
</script>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?php

echo monta_cabecalho_relatorio('95');
echo cabecalhoObra($_SESSION['obras2']['obrid'], 'simples');
monta_titulo( 'Restrições e Inconformidades', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );

?>
<form method="post" id="formulario" name="formulario" onsubmit="return validar(<?php echo $_REQUEST["rstid"]; ?>);" action="" enctype="multipart/form-data">
    
        <input type="hidden" name="requisicao" id="requisicao" value="salvar"/>
        <input type="hidden" name="arqid"      id="arqid"      value=""/>
        <input type="hidden" name="subimete"   id="subimete"   value='2'/>
        
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%" id="tabela_dados">
		<tr>
			<td class="SubTituloDireita" align="right">Item:</td>
			<td valign="top">
				<div style="display: inline;">
					<input type="radio" name="rstitem" id="rstitem_r" value="R" <?php echo ($rstitem == 'R' ? 'checked="checked"' : '') ?> <?php echo ($habilitadoConteudo == true ? '' : 'disabled="disabled"') ?>>
					<label for="rstitem_r">Restrição</label>
					<input type="radio" name="rstitem" id="rstitem_i" value="I" <?php echo ($rstitem == 'I' ? 'checked="checked"' : '') ?> <?php echo ($habilitadoConteudo == true ? '' : 'disabled="disabled"') ?>>
					<label for="rstitem_i">Inconformidade</label>
				</div>
			</td>
                        <?php
                        if($docid != NULL ){
                            echo '<td rowspan="6">' ;
                                if( possui_perfil(PFLCOD_CALL_CENTER)){
                                    wf_desenhaBarraNavegacao($docid, array('rstid' =>  $rstid), array('acoes' => true));
                                }else{
                                    wf_desenhaBarraNavegacao($docid, array('rstid' =>  $rstid));
                                }
                            echo '</td>';
                        }else{
                            if(!empty($rstid)){
                                $restrict = new Restricao();
                                $docid    = $restrict->atualizaDocidNullRetricao($rstid, true);

                                if($docid){
                                   echo '<td rowspan="6">';
                                    wf_desenhaBarraNavegacao($docid, array('rstid' =>  $rstid));
                                    echo '</td>'; 
                                }else{
                                    echo '<td rowspan="6"> &nbsp; </td>';
                                }
                            }else{
                                echo '<td rowspan="6"> &nbsp; </td>';
                            }
                        }
                        
                        ?>
		</tr>
		<tr>
			<td class="SubTituloDireita" align="right">Situação da Obra na Ocorrência:</td>
			<td valign="top">
			<?php
				$db->monta_combo( 'fsrid', $faseRestricao->listaCombo(), $somenteLeituraConteudo, 'Selecione', '', '', '', '115', 'S', 'fsrid');   
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" align="right">Tipo:</td>
			<td valign="top">
                <select <?= ($somenteLeituraConteudo == 'N') ? 'name="tprid_disable" disabled="disabled"' : 'name="tprid"'; ?> class="CampoEstilo obrigatorio" style="width: auto" id="tprid">
                    <option value="">Selecione</option>
                    <? foreach($tipoRestricao->listaComboFlag() as $key => $value): ?>
                        <option liberaanexo="<?=$value['liberaanexo']?>" <?= ($value['codigo'] == $tprid) ? 'selected="selected"' : ''?> value="<?=$value['codigo']?>"><?=$value['descricao']?></option>
                    <? endforeach; ?>
                </select>
			</td>
		</tr>
	    <tr>
	        <td align='right' class="SubTituloDireita">Descrição:</td>
	        <td>
	        <?php $rstdsc = $restricao->rstdsc; ?>
			<?=campo_textarea("rstdsc",'N',$somenteLeituraConteudo,'', 70, 8, 5000);?>
		    </td>
	    </tr>
	    <tr>
	        <td align='right' class="SubTituloDireita">Previsão da Providência:</td>
	        <td valign="top">
	        <?php $rstdtprevisaoregularizacao = formata_data($restricao->rstdtprevisaoregularizacao); ?>
		    <?= campo_data2( 'rstdtprevisaoregularizacao', 'N', $somenteLeituraConteudo, '', 'N' ); ?>
		    </td>
	    </tr>    
	    <tr>
	        <td align='right' class="SubTituloDireita">Providência:</td>
	        <td>
	        <?php $rstdscprovidencia = $restricao->rstdscprovidencia; ?>
			<?=campo_textarea("rstdscprovidencia", 'N', $somenteLeituraConteudo, '', 70, 8, 5000);?>
		    </td>
	    </tr>
            
	    <?php
            if($esdid == ESDID_AGUARDANDO_ANALISE_FNDE && possui_perfil( array(PFLCOD_GESTOR_MEC, PFLCOD_SUPERVISOR_MEC, PFLCOD_SUPER_USUARIO)) ){
            ?>            
                <tr>
                    <td class="SubTituloDireita" align="right">Ressalva:</td>
                    <td valign="top">
                            <div style="display: inline;">
                                    <input type="radio" name="rstflressalva" id="rstflressalva_s" value="S" <?php echo ($rstflressalva == 'S' ? 'checked="checked"' : '') ?> <?php echo ($habilitadoConteudo == true ? '' : 'disabled="disabled"') ?>>
                                    <label for="rstflressalva_s">Sim</label>
                                    <input type="radio" name="rstflressalva" id="rstflressalva_n" value="N" <?php echo ($rstflressalva == 'N' ? 'checked="checked"' : '') ?> <?php echo ($habilitadoConteudo == true ? '' : 'disabled="disabled"') ?>>
                                    <label for="rstflressalva_n">Não</label>
                            </div>
                    </td>
                </tr>
                <tr>
                    <td align='right' class="SubTituloDireita">Observação da Ressalva:</td>
                    <td>
                    <?php $rstobsressalva = $restricao->rstobsressalva; ?>
                            <?=campo_textarea("rstobsressalva",'N',$somenteLeituraConteudo,'', 70, 8, 5000, '', 0, '', false, NULL);?>
                        </td>
                </tr>            
	    <?php 
            }elseif($esdid == ESDID_SUPERADA){
                ?>
                
                <tr>
                    <td class="SubTituloDireita" align="right">Ressalva:</td>
                    <td valign="top">
                            <div style="display: inline;">
                                    <input type="radio" name="rstflressalva" id="rstflressalva_s" value="S" <?php echo ($rstflressalva == 'S' ? 'checked="checked"' : '') ?> <?php echo ($habilitadoConteudo == true ? '' : 'disabled="disabled"') ?> >
                                    <label for="rstflressalva_s">Sim</label>
                                    <input type="radio" name="rstflressalva" id="rstflressalva_n" value="N" <?php echo ($rstflressalva == 'N' ? 'checked="checked"' : '') ?> <?php echo ($habilitadoConteudo == true ? '' : 'disabled="disabled"') ?>>
                                    <label for="rstflressalva_n">Não</label>
                            </div>
                    </td>
                </tr>
                <tr>
                    <td align='right' class="SubTituloDireita">Observação da Ressalva:</td>
                    <td>
                    <?php $rstobsressalva = $restricao->rstobsressalva; ?>
                            <?=campo_textarea("rstobsressalva",'N','N','', 70, 8, 5000, '', 0, '', false, NULL, NULL);?>
                        </td>
                </tr>
                
                
        <?php
            }

            $sql = "

                SELECT
                    a.aeddscrealizada,
                    TO_CHAR(h.htddata, 'DD/MM/YYYY às HH24:MI:SS') as htddata,
                    u.usunome,
                    c.cmddsc,
                    CASE WHEN a.aedid IN (2653,3467,2652,2654,2655) THEN 'FNDE' ELSE 'MUN' END entidade
                FROM workflow.documento d
                JOIN workflow.historicodocumento h ON h.docid = d.docid AND h.aedid IN (2653,3467,2652,2654,2655,2656)
                JOIN workflow.comentariodocumento  c ON c.hstid = h.hstid
                JOIN workflow.acaoestadodoc a ON a.aedid = h.aedid
                JOIN seguranca.usuario u ON u.usucpf = h.usucpf
                WHERE d.docid = {$restricao->docid}
                ORDER BY h.hstid ASC
            ";
            if($restricao->docid)
                $historico = $db->carregar($sql);
            if(!empty($historico)){
        ?>
            <tr>
                <td align='right' class="SubTituloDireita">Observações do Trâmite da Restrição/Inconformidade:</td>
                <td>
                    <style>
                        .box-comentario{
                            padding: 0 0 0 2px;
                            border: 1px solid #ccc;
                            color: #7B7B7B;
                            margin: 1px;
                            width: 478px;
                            -webkit-border-radius: 3px;
                            -moz-border-radius: 3px;
                            border-radius: 3px;
                        }
                        .box-comentario-fnde{
                            border: 2px solid #97BAF9;
                            color:  #003DAB;
                            width: 476px;
                        }
                    </style>
                    <? foreach($historico as $item): ?>
                        <div style="" title="<?if($item['entidade'] == 'FNDE'):?>Resposta FNDE<?else:?>Resposta Entidade<? endif;?>" class="box-comentario  <?if($item['entidade'] == 'FNDE'):?>box-comentario-fnde<? endif;?>">
                            <p style="font-size:10px"><b style="font-size:12px"><?=$item['aeddscrealizada']?></b> por <?=$item['usunome']?> (<?=$item['htddata']?>):</p>
                            <p><b style="font-size:12px">"<?=$item['cmddsc']?>"</b></p>
                        </div>
                    <? endforeach; ?>
                </td>
            </tr>
        <?php
            }
	    
            if($habilitadoBotao || ($esdid == ESDID_AGUARDANDO_PROVIDENCIA || $esdid == ESDID_AGUARDANDO_CORRECAO)){ 
                echo '<tr id="arquivos">
                          <td align="right" class="SubTituloDireita">Arquivos:</td>
                          <td colspan="2">
                              <span id="addArquivo" style="cursor:pointer;"> 
                                <img src="../imagens/obras/incluir.png" width="20px" border="0"> Adicionar Arquivo 
                              </span>
                          </td>
                      </tr>';
            }
	    ?>

            <tbody id="camposDinamicosArquivos">
                
            </tbody>
                
            <tr bgcolor="#C0C0C0">
                    <td colspan="3">

                    <center>
                        <?php 
                            if($habilitadoBotao || ($esdid == ESDID_AGUARDANDO_PROVIDENCIA || $esdid == ESDID_AGUARDANDO_CORRECAO)){ 
                                echo "<input type='submit' class='botao' name='Salvar' value='Salvar'/>";
                            } 
                            
                            if($habilitadoBotao == false && ($esdid == ESDID_AGUARDANDO_PROVIDENCIA || $esdid == ESDID_AGUARDANDO_CORRECAO)){
                                $only_file = '<input type="hidden" name="only_file" id="only_file" value="true"/>';
                            }else{
                                $only_file = '<input type="hidden" name="only_file" id="only_file" value="false"/>';
                            }
                            echo $only_file;
                        ?>
                        <input type='button' class='botao' value='Voltar' id='btFechar' name='btFechar' onclick='fechaJanela();'/>
                    </center>
                    </td>
            </tr>
                
	</table>
        <?php

            if($tipoRestricao->liberaanexo == 't')
                carregaListaArquivo($rstid);
        ?>
</form>
    
<?php
    $objObras = new Obras($_SESSION['obras2']['obrid']);
    $blockEdicao = $objObras->verificaObraVinculada();
    if($blockEdicao){
        echo '<script type="text/javascript">';
        echo " setTimeout(bloqueiaForm('formulario'), 500);
               function bloqueiaForm(idForm){
                  jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
                  jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                  jQuery('#btFechar').attr('disabled', false);
               }
             ";
        echo '</script>';
    }
?>

<script type="text/javascript">

    $(function(){
        $('#tprid').change(function(){
            if($(this).find('option:selected').attr('liberaanexo') == 't'){
                $('#arquivos').show();
                $('.tabela_arquivos').show();
            } else {
                $('#arquivos').hide();
                $('.tabela_arquivos').hide();
            }
        }).change();
    });

function validar(rstid){
	
	var mensagem           = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
	var validacao          = true;
	var tprid              = $('#tprid').val(); //document.getElementById("tprid").value;
	var rstdsc	       = document.getElementById("rstdsc");
	var fsrid	       = document.getElementById("fsrid");
	var rstdscprovidencia  = document.getElementById("rstdscprovidencia");
        
        if($('#requisicao').val() == 'download' && $('#arqid').val() != ''){
            return true;
        }
        if($('#requisicao').val() == 'excluirArquivo' && $('#arqid').val() != ''){
            return true;
        }
        
        if($('[name=rstflressalva]:checked').val() == 'S' && $('#rstobsressalva').val().trim().length == 0 ){
            alert('Restrição/Inconformidade com Ressalva, preencha o campo de observação.');
            $('#rstobsressalva').focus();
            return false;
        };
        
	if (rstid){
		var rstsituacao	   = document.getElementById("rstsituacao");
	}
	
	rstdsc.value = tinyMCE.getContent("rstdsc");
	var rstdsc_l = rstdsc.value.length;
	
	rstdscprovidencia.value = tinyMCE.getContent("rstdscprovidencia");
	var rstdscprovidencia_l = rstdscprovidencia.value.length;
	
	var rstitemVal = $('[name=rstitem]:checked').val() || '';
	if( rstitemVal == "" ){
		mensagem += 'Item \n';
		validacao = false;
	}
	
	if( fsrid.value == "" ){
		mensagem += 'Situação da Obra na Ocorrência \n';
		validacao = false;
	}

	if (tprid == "" && tprid.length == 0){
		mensagem += 'Tipo \n';
		validacao = false;
	}
	
	if (rstdsc.value == ""){
		mensagem += 'Descrição \n';
		validacao = false;
	}
	
	if (rstdsc_l > 5000){
		alert("O limite da Descrição é de 5000 caracteres foi ultrapassado");
		return false;
	}
	
	if ( rstdscprovidencia_l > 5000 ){
		alert('O campo Providência deve ter no máximo 5000 caracteres!');
		return false;
	}

	if (document.formulario.rstdtprevisaoregularizacao.value != ""){
		if (!validaData(document.formulario.rstdtprevisaoregularizacao)){
			alert("A data de previsão de regularização informada é inválida");
			document.formulario.rstdtprevisaoregularizacao.focus();
			return false;
		}
	}	
	
	if (!validacao){
		alert(mensagem);
	} 
	
	return validacao; 
}
</script>

</body>


<?php

function carregaCampoArquivo(){
    global $db;
    
?>    
    
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela tabela_arquivos" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloDireita" width="20%">Descrição:</td>
            <td><?php echo campo_texto('rardesc[]','S',$habilita,'',43,100,'','', '', '', '', 'id="oardesc[]"', '');?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Tipo:</td>
            <td>
            <?php
                $tipoArquivo = new TipoArquivo();
                $sql = $tipoArquivo->listaCombo();
                $db->monta_combo("tpaid[]", $sql, 'S', "Selecione...", "", '', '', '', 'S', 'tpaid');
            ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Arquivo:</td>
            <td>
                <input type="file" name="arquivo[]" class="obrigatorio"/>
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>
    </table>
    
<?php
}

function carregaListaArquivo($rstid){
    
    $ri = new Restricao();
    $dados = $ri->getListaArquivosRestricaoInconformidade($rstid);
    
    $html = '';
    
    if(count($dados) > 0){
        echo '<div class="tabela_arquivos">';
        monta_titulo( 'Lista de arquivos de Restrições e Inconformidades', '');
        $html .= '<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">';
        $html .= '<tr>';
        $html .= '<th>Ação</th>';
        $html .= '<th>Tipo Arquivo</th>';
        $html .= '<th>Descrição</th>';
        $html .= '<th>Arquivo</th>';
        $html .= '<th>Data de Inclusão</th>';
        $html .= '<th>Gravado por</th>';
        $html .= '</tr>';
        
        $counter = 0;
        $str_bg = '';
        foreach ($dados as $key => $value) {
            $counter++;
            $str_bg = ($counter % 2 == '') ? '' : ' bgcolor="#FFFFFF" ';
            
            $html .= '<tr '. $str_bg .'>';
            
            $html .= '<td>'.$value['acao'].'</td>';
            $html .= '<td>'.$value['tpadesc'].'</td>';
            $html .= '<td>'.$value['descricao'].'</td>';
            $html .= '<td>'.$value['nome_arquivo'].'</td>';
            $html .= '<td>'.$value['data'].'</td>';
            $html .= '<td>'.$value['usunome'].'</td>';
            
            $html .= '</tr>';
        }
    }
    echo $html;
    echo '</div>';
}

function download(){
	
	require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	$file = new FilesSimec(null,null,'obras2');
	$file->getDownloadArquivo( $_REQUEST['arqid'] );
	
	die('<script type="text/javascript">
                document.location.href = document.location.href;
             </script>');
}

function excluirArquivo(){
    $arqid = $_POST['arqid'];
    
    $ri = new Restricao();
    $resp = $ri->excluirArquivosRestricaoInconformidade($arqid);
    
    if($resp){
        die('<script type="text/javascript">
                alert(\'Arquivo excluido com sucesso.\');
                document.location.href = document.location.href;
             </script>');
    }else{
        die('<script type="text/javascript">
                alert(\'Erro ao excluir o arquivo.\');
                document.location.href = document.location.href;
             </script>');
    }
    
}

?>