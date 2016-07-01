<?php
// captura dados do projeto da requisição
//$atiid = $_REQUEST['atiid'] ? (integer) $_REQUEST['atiid'] : PROJETO;

// captura o id da obra
$obrid  = ($_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid']);

// captura o id da etapa
$icoid  = ($_REQUEST['icoid'] ? $_REQUEST['icoid'] : $_SESSION['obras2']['icoid']);
$_SESSION['obras2']['icoid'] = $icoid;

if ( empty($icoid) || empty($obrid) ){
    die("<script>
            alert('Faltam parametros para acessar esta tela!');
            history.go(-1);
         </script>");
}

$sql = "SELECT
            icovlritem
        FROM
            obras2.itenscomposicaoobra
        WHERE
            icoid = {$icoid};";

$icovlritem = $db->pegaUm($sql);

if ( $_POST['evt'] == 'salvar' ){

//    if ( $_POST['atualizarNivel'] == 1 ){
//        updateLevelCronograma( $_POST['vl_etapa_atualizado'], $atiid, $icoid, 'detalhamento');
//    }

	//verificar se as data de inicio da estrutura precisa ser alterada
    if( !empty($_POST['dtInicioEstrutura'])){
        $arObra['obrdtinicio'] = formata_data_sql($_POST['dtInicioEstrutura']);
    }

    //verificar se as data de termino da estrutura precisa ser alterada
    if( !empty($_POST['dtTerminoEstrutura'])){
        $arObra['obrdtfim'] = formata_data_sql($_POST['dtTerminoEstrutura']);
    }

    $estrutura = new Obras($obrid);
    $estrutura->popularDadosObjeto( $arObra )->salvar();

    //verificar se as data de inicio da etapa precisa ser alterada
    if( !empty($_POST['dtInicioEtapa'])){
        $arEtapa['icodtinicioitem'] = formata_data_sql($_POST['dtInicioEtapa']);
    }

    //verificar se as data de termino da etapa precisa ser alterada
    if( !empty($_POST['dtTerminoEtapa'])){
        $arEtapa['icodterminoitem'] = formata_data_sql($_POST['dtTerminoEtapa']);
    }

    $etapa = new ItensComposicaoObras($icoid);
    $etapa->popularDadosObjeto( $arEtapa )->salvar();

    $detalheitem = new DetalheItem();

    $arDitid = $detalheitem->getAllID($icoid);
    $_POST['ditid'] = (is_array( $_POST['ditid'] ) ? $_POST['ditid'] : array());
    $arDitid = array_diff($arDitid, $_POST['ditid']);

    $detalheitem->desabilitar( $arDitid );

    $prev = new PrevisaoDetalheItem();
    $prev->desabilitar( $arDitid );

    for ($i=0; $i < count( $_POST['ditdsc'] ); $i++){

        if(!empty($_POST['ditidPai'][$i]) ){
        	if(is_numeric($_POST['ditidPai'][$i])){
	        	$ditidpai = $_POST['ditidPai'][$i];
        	}else{
        		$indice = substr($_POST['ditidPai'][$i], 1);
        		$ditidpai = $_POST['ditid'][$indice];
        	}
        }else{
        	$ditidpai = null;
        }

        $arDados = array(
                          'ditid'          => ($_POST['ditid'][$i] ? $_POST['ditid'][$i] : null),
                          'ditidpai'       => $ditidpai,
                          'icoid'          => $icoid,
                          'ditdsc'         => $_POST['ditdsc'][$i],
                          'ditstatus'      => 'A',
                          'ditdatainicio'  => ($_POST['ditdatainicio'][$i] ? formata_data_sql( $_POST['ditdatainicio'][$i] ) : null),
                          'ditdatatermino' => ($_POST['ditdatatermino'][$i] ? formata_data_sql( $_POST['ditdatatermino'][$i] ) : null),
                          'ditvalor'       => ($_POST['ditvalor'][$i] ? desformata_valor( $_POST['ditvalor'][$i] ) : null),
                          'ditpercentual'  => ($_POST['ditpercentual'][$i] ? desformata_valor( $_POST['ditpercentual'][$i] ) : null),
                          'ditordem'       => ($i + 1),
                          'ditmetafisica'  => ($_POST['ditmetafisica'][$i] ? desformata_valor( $_POST['ditmetafisica'][$i] ) : null),
                          'umcid'          => ($_POST['umcid'][$i] ? $_POST['umcid'][$i] : null)
                        );

        $arCamposNull = array();
        if ( empty( $_POST['ditmetafisica'][$i] ) || empty( $_POST['umcid'][$i] ) ){
            $arCamposNull = array("ditmetafisica", "umcid");
            $arDados['ditmetafisica'] = null;
            $arDados['umcid']         = null;
        }
        
        foreach ($arDados as $key => $value) {
            if($value == null){
                array_push($arCamposNull, $key);
                $arDados[$key] = null;
            }
        }

        $_POST['ditid'][$i] = $detalheitem->popularDadosObjeto( $arDados )->salvar(true, true, $arCamposNull);
        $detalheitem->clearDados();
    }

    $detalheitem->commit();

//    $vlrObra = new ValorMensalObra();
//    $vlrObra->calculaCronograma($atiid);

    die("<script>
            alert('Operação realizada com sucesso!');
            window.location = '?modulo=principal/detalhe_da_etapa&acao=A&icoid={$icoid}';
         </script>");
}

// Inclusão de arquivos padrão do sistema
include APPRAIZ . 'includes/cabecalho.inc';
// Cria as abas do módulo
echo '<br>';
$db->cria_aba(ID_ABA_DETALHAMENTO_ITEM_OBRA,$url,$parametros);

echo cabecalhoObra($obrid);
echo '<br>';

// Cria o título da tela
monta_titulo( $titulo_modulo, '<b>Após inserir, atualizar ou excluir um detalhamento da etapa, clique em salvar.</b>' );

//cabecalho_projeto($atiid);
//cabecalho_estrutura( $obrid );
//cabecalho_etapa( $icoid );
// echo cabecalhoEstrutura($atiid,$obrid,$icoid);
echo cabecalhoCronograma( $obrid, $icoid );
?>
<form method="post" id="formDetalhamento">
<input type="hidden" name="evt" id="evt" value="">
<input type="hidden" name="dtInicioEstrutura" id="dtInicioEstrutura" value="">
<input type="hidden" name="dtTerminoEstrutura" id="dtTerminoEstrutura" value="">
<input type="hidden" name="dtInicioEtapa" id="dtInicioEtapa" value="">
<input type="hidden" name="dtTerminoEtapa" id="dtTerminoEtapa" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td class="SubTituloTelaCentro">Lista dos Detalhamentos da Etapa</td>
    </tr>
    <tr>
        <td>
            <table id="lista_detalhamento" width="95%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
                <thead>
                    <tr id="cabecalho">
                        <td width="5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ordem</strong></td>
                        <td width="5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
                        <td width="30%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Descrição</strong></td>
                        <td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Quantidade</strong></td>
                        <td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Unidade de Medida</strong></td>
                        <td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data de Início</strong></td>
                        <td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data de Término</strong></td>
                        <td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor do Item (R$)</strong></td>
                        <td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>(%) Referente a Etapa <br/></strong></td>
                    </tr>
                </thead>
                <tbody id="tr_matrix" style="display:none;">
	                <tr>
	                    <td align="center"></td>
	                    <td align="center" nowrap="nowrap">
	                        <span>
	                            <img src='/imagens/excluir.gif' style='cursor:pointer;' border='0' title='Excluir'>
	                            <img src='/imagens/gif_inclui.gif' style='cursor:pointer;' border='0' title='Adicionar filho'>
	                            <img src='/imagens/money.gif' style='cursor:pointer;' border='0' title='Detalhar valor previsto'>
	                        </span>
	                    </td>
	                    <td nowrap="nowrap">
	                        <input type="hidden" name="ditid[]" value="">
	                        <input type="hidden" name="ditidPai[]" value="">
	                        <?php echo campo_texto( 'ditdsc[]', 'S', 'S', '', 70, 120, '', '', '', '', 0); ?>
	                    </td>
	                    <td align="center"><?php echo campo_texto( 'ditmetafisica[]', 'N', 'S', '', 8, 12, '[###.]###,##', '', 'right', '', 0); ?></td>
	                    <td align="center">
	                    <?php
	                    $unidadeMedidaCronograma = new UnidadeMedidaCronograma();
	                    $dados = $unidadeMedidaCronograma->recuperarTodos("umcid AS codigo, umcdsc AS descricao", array("umcstatus = 'A'"), "umcdsc");
	                    echo $db->monta_combo( 'umcid[]', $dados, 'S', 'Selecione...', '', '','','120','N' );
	                    ?>
	                    </td>
	                    <td align="center"><?php echo campo_data2( 'ditdatainicio[]', 'S', 'S', '', 'S','','verificaDataInicio(this);','','verificaDataInicio(this);' ); ?></td>
	                    <td align="center"><?php echo campo_data2( 'ditdatatermino[]', 'S', 'S', '', 'S','','verificaDataTermino(this);','','verificaDataTermino(this);' ); ?></td>
	                    <td align="center"><?php echo campo_texto( 'ditvalor[]', 'S', 'S', '', 20, 14, '', '', 'right', '', 0, 'onkeypress="calculoDetalhamento.setDefaultValue( this )";', 'calculoDetalhamento.manager( this );'); ?></td>
	                    <td align="center"><?php echo campo_texto( 'ditpercentual[]', 'N', 'N', '', 5, 6, '', '', 'right', '', 0); ?> %</td>
	                </tr>
                </tbody>
                <tr id="tr_total" bgcolor="#FFFFFF">
                    <td align="right" colspan="7" valign="top"><strong>Total</strong></td>
                    <td align="center" valign="top">
                        <input class='disabled' style="text-align:right" type='text' id='totalv' size='20' maxlength='14' value='<?php echo number_format($somav,2,',','.'); ?>' disabled="disabled">
                    </td>
                    <td align="center" valign="top">
                        <input class='disabled' style="text-align:right" type='text' id='total' size='6' maxlength='6' value='<?php echo number_format($soma,2,',','.'); ?>' disabled="disabled"> %
                    </td>
                </tr>
                <tr id="tr_vlcontrato" bgcolor="#FFFFFF">
                    <td align="right" colspan="7" valign="top"><strong>Valor da Etapa</strong></td>
                    <td align="center" valign="top">
                        <input class='disabled' style="text-align:right" type='text' name='vl_etapa' id='vl_etapa' size='20' maxlength='14' value='<?php echo number_format($icovlritem,2,',','.'); ?>' disabled="disabled">
                        <input type='hidden' name='vl_etapa_atualizado' id='vl_etapa_atualizado' value=''>
                        <input type='hidden' name='atualizarNivel' id='atualizarNivel' value=''>
                    </td>
                    <td align="center" valign="top">
                        <input class='disabled' style="text-align:right" type='text' id='vl_porcento' size='6' maxlength='6' value='100,00' disabled="disabled"> %
                    </td>
                </tr>
                <tr id="tr_vlrestante" bgcolor="#FFFFFF">
			        <td colspan="6">
			            <a href="#" onclick="gerenteDetalhamento.addLine();">
			                <img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir Detalhamento">&nbsp;&nbsp;Inserir Detalhamento
			            </a>
			        </td>
                    <td align="right" valign="top"><strong>Valor Restante</strong></td>
                    <td align="center" valign="top">
                        <input class='disabled' style="text-align:right" type='text' id='rest_totalv' size='20' maxlength='14' value='<?php echo number_format($icovlritem-$somav,2,',','.'); ?>' disabled="disabled">
                    </td>
                    <td align="center" valign="top">
                        <input class='disabled' style="text-align:right" type='text' id='rest_total' size='6' maxlength='6' value='<?php echo number_format(100-$soma,2,',','.'); ?>' disabled="disabled"> %
                    </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td>

        </td>
    </tr>
    <tr bgcolor="#C0C0C0">
        <td>
            <div style="float: left;">
                <input type="button" id="salvar" value="Salvar" style="cursor: pointer" onclick="validaForm();">
                <? if($_GET['croid']): ?>
                    <input type="button" value="Voltar" style="cursor: pointer" onclick="location.href='?modulo=principal/editar_cronograma&acao=A'">
                <? else: ?>
                    <input type="button" value="Voltar" style="cursor: pointer" onclick="location.href='?modulo=principal/etapas_da_obra&acao=A'">
                <? endif; ?>
            </div>
        </td>
    </tr>
</table>
</form>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>

<script type="text/javascript">

function verificaDataInicio(data){
    var obData        = new Data();
    var flgEdificacao = false;
    var flgEtapa      = false;

    if( obData.comparaData(data.value,$('#crtdatainicioLabel').html(),'<') ){
        alert('A data de início não pode ser menor que a data de início do contrato.');
        data.value = '';
        data.focus();
        return;
    }

    if( obData.comparaData(data.value,$('#crtdatafimLabel').html(),'>') ){
        alert('A data de início não pode ser maior que a data de término do contrato.');
        data.value = '';
        data.focus();
        return;
    }

    // verificando se a data de inicio não é maior que a data de termino
    var nome = ($(data).attr('id')).split('_');
    if(nome[1]){
        if( obData.comparaData(data.value,$('#ditdatatermino_'+nome[1]).val(),'>') ){
            alert('A data de início não pode ser maior que a data de término.');
            data.value = '';
            data.focus();
            return;
        }
    }

    //verificando se será necessario atualizar a data de inicio da edificação
//    if( obData.comparaData(data.value,$('#eobinicioLabel').html(),'<') ){
//        flgEdificacao = true;
//    }

    //verificando se será necessario atualizar a data de inicio da etapa
    if( obData.comparaData(data.value,$('#icodtinicioitemLabel').html(),'<') ){
        flgEtapa = true;
    }

    //caso alguma atualização for necessaria
    if( flgEdificacao || flgEtapa ){
        if(confirm("A data de início deste detalhamento é maior que a data de início da etapa, deseja alterá-la?")) {
//            if(flgEdificacao){
//                $('#eobinicioLabel').html(data.value);
//                $('#eobinicioLabel').css('color','red');
//                $('#dtInicioEstrutura').val(data.value);
//            }
            if(flgEtapa){
                $('#icodtinicioitemLabel').html(data.value);
                $('#icodtinicioitemLabel').css('color','red');
                $('#dtInicioEtapa').val(data.value);
            }
        }else{
            data.value = '';
        }
    }
}

function verificaDataTermino(data){
    var obData        = new Data();
    var flgEdificacao = false;
    var flgEtapa      = false;

    if( obData.comparaData(data.value,$('#crtdatainicioLabel').html(),'<') ){
        alert('A data de término não pode ser menor que a data de início do contrato.');
        data.value = '';
        data.focus();
        return;
    }

    if( obData.comparaData(data.value,$('#crtdatafimLabel').html(),'>') ){
        alert('A data de término não pode ser maior que a data de término do contrato.');
        data.value = '';
        data.focus();
        return;
    }

    // verificando se a data de termino não é menor que a data de inicio
    var nome = ($(data).attr('id')).split('_');
    if(nome[1]){
        if( obData.comparaData(data.value,$('#ditdatainicio_'+nome[1]).val(),'<') ){
            alert('A data de término não pode ser menor que a data de início.');
            data.value = '';
            data.focus();
            return;
        }
    }

    //verificando se será necessario atualizar a data de termino da edificação
//    if( obData.comparaData(data.value,$('#eobterminoLabel').html(),'>') ){
//        flgEdificacao = true;
//    }

    //verificando se será necessario atualizar a data de termino da etapa
    if( obData.comparaData(data.value,$('#icodterminoitemLabel').html(),'>') ){
        flgEtapa = true;
    }

    //caso alguma atualização for necessaria
    if( flgEdificacao || flgEtapa ){
        if(confirm("A data de término deste detalhamento é maior que a data de término da etapa, deseja alterá-la?")) {
//            if(flgEdificacao){
//                $('#eobterminoLabel').html(data.value);
//                $('#eobterminoLabel').css('color','red');
//                $('#dtTerminoEstrutura').val(data.value);
//            }
            if(flgEtapa){
                $('#icodterminoitemLabel').html(data.value);
                $('#icodterminoitemLabel').css('color','red');
                $('#dtTerminoEtapa').val(data.value);
            }
        }else{
            data.value = '';
        }
    }
}

function gerenteDetalhamento(){

	var indTR = ($('#lista_detalhamento tr').length - 5);

	var indTRFilha = ($('id^="tr_detalhamento_filha_"').length);

	var imgDesabUp = $( document.createElement('img') ).attr({src    : '/imagens/seta_cimad.gif',
                                                              border : 0,
                                                              title  : 'Sobir'});
	var imgUp = $( document.createElement('img') ).attr({src    : '/imagens/seta_cima.gif',
		                                                 border : 0,
		                                                 title  : 'Sobir'});

	var imgDesabDown = $( document.createElement('img') ).attr({src    : '/imagens/seta_baixod.gif',
		                                                      border : 0,
		                                                      title  : 'Descer'});
	var imgDown = $( document.createElement('img') ).attr({src    : '/imagens/seta_baixo.gif',
		                                                 border : 0,
		                                                 title  : 'Descer'});

	this.addLine = function (dataLine,idTRPai){
		dataLine = dataLine || '';
		idTRPai  = idTRPai || '';

		if(idTRPai){
		        var idTR = 'tr_detalhamento_filha_' + indTR;
		        var clone = $('#tr_matrix tr').clone()
		                                   .attr('id', idTR)
		                                   .show();

                if ( IsNumeric(idTRPai) ){

                    clone.attr('idPai', 'tr_detalhamento_' + idTRPai);

                    $('#tr_detalhamento_' + idTRPai).find('[name*=ditmetafisica]:first')
                                                  .val('')
                                                  .hide();

                    $('#tr_detalhamento_' + idTRPai).find('[name*=umcid]:first')
                                                  .val('')
                                                  .hide();
                }else{

                    clone.attr('idPai', idTRPai);

                    $('#' + idTRPai).find('[name*=ditmetafisica]:first')
                                    .val('')
                                    .hide();

                    $('#' + idTRPai).find('[name*=umcid]:first')
                                    .val('')
                                    .hide();
                }
		}else{
		        if(dataLine.ditid){
			        var idTR = 'tr_detalhamento_' + dataLine.ditid;
		        }else{
			        var idTR = 'tr_detalhamento_new_' + indTR + '';
		        }
		        var clone = $('#tr_matrix').clone()
		                                   .attr('id', idTR)
		                                   .show();
		}

        clone.html( replaceAll (clone.html() , 'id="ditdatainicio[]"', 'id="ditdatainicio_' + indTR + '"') );
        clone.html( replaceAll (clone.html() , "document.getElementById('ditdatainicio[]')", "document.getElementById('ditdatainicio_" + indTR + "')") );

        clone.html( replaceAll (clone.html() , 'id="ditdatatermino[]"', 'id="ditdatatermino_' + indTR + '"') );
        clone.html( replaceAll (clone.html() , "document.getElementById('ditdatatermino[]')", "document.getElementById('ditdatatermino_" + indTR + "')") );

		if(idTRPai){
            $(clone).find('td:eq(1) span img:eq(1)').remove();

            //criando a seta de filho
    		var imgSeta = $( document.createElement('img') ).attr({src    : '/imagens/seta_filho.gif', border : 0});
    		$(clone).find('[name*=ditdsc]').attr("size", "71");
    		$(clone).find('[name*=ditdsc]').before( imgSeta );
    		$(imgSeta).before( '&nbsp;' );
            if(IsNumeric(idTRPai)){
            	   $('#tr_detalhamento_'+idTRPai+' tr:last').after( clone );
    		       $(clone).find('[name*=ditidPai]').val( idTRPai );
            }else{
            	   $('#'+idTRPai+' tr:last').after( clone );
            	   if( IsNumeric(idTRPai.split('_')[2])){
    			       $(clone).find('[name*=ditidPai]').val( idTRPai.split('_')[2] );
    	           }else{
    			       $(clone).find('[name*=ditidPai]').val( "_"+idTRPai.split('_')[3] );
            	   }
            }
		}else{
            prepareAddChildrenImg( clone, idTR );
            $('#tr_matrix').before( clone );
		}

		prepareValueDetailImg( clone, dataLine );

        prepareRemoveImg( clone, idTR );

		if ( dataLine != '' ){
		    loadData( dataLine, clone );
		}

		prepareArrowImg();
	    colorManager();
	    indTR++;
	}


	this.removeLine = function ( id ){
	    enableMetaAndUnidade( id );
	    $( '#' + id ).remove();
	    colorManager();
	    prepareArrowImg();

	    calculoDetalhamento.manager();
	}

    function enableMetaAndUnidade(id){
        if ( id.indexOf('filha') >= 0 ){
            if ( $( '#' + id ).parent('tbody').find('tr').length == 2 ){
            	$( '#' + id ).parent('tbody')
            	             .find('tr:first')
            	             .find('[name*=ditmetafisica]:first')
                             .val('')
                             .show();

            	$( '#' + id ).parent('tbody')
            	             .find('tr:first')
            	             .find('[name*=umcid]:first')
                             .val('')
                             .show();
            }
        }
    }

    this.moveUp = function ( tr ){
    	tr = $( tr );
    	tr.insertBefore( tr.prev() );
    	colorManager();
    	prepareArrowImg();
    }

    this.moveDown = function ( tr ){
    	tr = $( tr );
    	tr.insertAfter( tr.next() );
    	colorManager();
    	prepareArrowImg();
    }

    function IsNumeric(input){
        var RE = /^-{0,1}\d*\.{0,1}\d+$/;
        return (RE.test(input));
    }

    function loadData( dataLine, line ){

        line.find('[name*=ditid]:not([name*=ditidPai])').val( dataLine.ditid );
        line.find('[name*=ditidPai]').val( dataLine.ditidPai );
        line.find('[name*=ditdsc]').val( dataLine.ditdsc );
        line.find('[name*=ditmetafisica]').val( mascaraglobal("[###.]###,##", dataLine.ditmetafisica) );
        line.find('[name*=umcid]').val( dataLine.umcid );

        line.find('[name*=ditdatainicio]').val( dataLine.ditdatainicio );
        line.find('[name*=ditdatatermino]').val( dataLine.ditdatatermino );
        line.find('[name*=ditvalor]').val( dataLine.ditvalor );

        calculoDetalhamento.manager( line.find('[name*=ditvalor]')[0] );

    }

	function prepareArrowImg (){
		var indMaxTr = ($('#lista_detalhamento').find('[id*=tr_detalhamento_]:not([id*=tr_detalhamento_filha])').length -1);
        $('#lista_detalhamento').find('[id*=tr_detalhamento_]:not([id*=tr_detalhamento_filha])').each(function (i, obj){

            var cloneImgUp = (i == 0
            		            ? $( imgDesabUp ).clone()
                    		    : $( imgUp ).clone()
                    		                .bind('click', function (){
                    		                    gerenteDetalhamento.moveUp( obj );
                        		                }));
            var cloneImgDown = (i == indMaxTr
            		            ? $( imgDesabDown ).clone()
                    		    : $( imgDown ).clone()
                                              .bind('click', function (){
                                                gerenteDetalhamento.moveDown( obj );
                                                }));

            $(obj).find('td:eq(0)').html('')
                                   .append( cloneImgUp )
                                   .append( '&nbsp;' )
                                   .append( cloneImgDown );
         });
	}

    function prepareRemoveImg( clone, idTR ){
        $(clone).find('td:eq(1) span img:eq(0)').bind('click', function() {
            if(confirm("Deseja remover esse registro e todos os relacionados à ele?")) {
	            gerenteDetalhamento.removeLine( idTR );
            }
        });
    }

    function prepareAddChildrenImg( clone, idTR ){
        $(clone).find('td:eq(1) span img:eq(1)').bind('click', function() {
        	gerenteDetalhamento.addLine(null,idTR);
        });
    }

    function prepareValueDetailImg( clone, dataLine ){
    	if (typeof dataLine == 'object' && dataLine.ditid != ''){
            $(clone).find('td:eq(1) span img:last').bind('click', function() {
            	location.href = '?modulo=principal/previsao_valor_detalhe&acao=A&ditid=' + dataLine.ditid;
            });
    	}else{
            $(clone).find('td:eq(1) span img:last').remove();
        }
    }

	function colorManager (){
	    var countLine = 0;
	    var colorLine = new Array('#FFFFFF', '#E0E0E0');
	    $('#lista_detalhamento').find('[id*=tr_detalhamento_]').each(function (i, obj){
	    	   $( obj ).attr('bgcolor', colorLine[countLine]);
	    	   countLine = (countLine == 0 ? 1 : 0);
		    });
	}
}

var gerenteDetalhamento = new gerenteDetalhamento();

function calculoDetalhamento(){
    var obCalc       = new Calculo();
    var percentTotal = new Number(0);
    var itemEdicao        = '';
    var vlrItemAnterior   = '';
    var vlrItemExcedido, vlrDetExcedido;

    this.manager = function (obj){
    	obj = obj || '';

    	if ( obj != '' ){
    	    formatarNumeroMonetario( obj );
    	}
        calculaTotal();
        calculaRestante();
    }

    this.setDefaultValue = function ( obj ){
        if ( obj != itemEdicao ){
            itemEdicao      = obj;
            vlrItemAnterior = obj.value;
        }
    }

    function calculaPercent(obj, ind, flgFilho){
    	flgFilho = flgFilho || false;
        var vlrEtapa = obCalc.converteMonetario( $('#vl_etapa').val() );


        var vlrItem     = obCalc.converteMonetario( obj.value );
        var percentItem = mascaraglobal("[###.]###,##", ((vlrItem / vlrEtapa) * 100).toFixed(2) );
        percentItem     = formatarNumeroMonetario( {value:percentItem} );
        $('[name*=ditpercentual]').eq(ind)
                                  .val( percentItem );

//        if(!flgFilho){
//	        percentTotal = (ind == 0 ? '0' : percentTotal);
//	        percentTotal = obCalc.operacao(percentTotal, percentItem, '+');
//	        percentTotal = formatarNumeroMonetario( {value:percentTotal} );
//	        $('#total').val( percentTotal );
//        }

//        var percentRest = obCalc.operacao('100', percentTotal, '-');
//        percentRest     = formatarNumeroMonetario( {value:percentRest} );
//        $('#rest_total').val( percentRest );
    }

    function calculaRestante(){
        var vlrRest = obCalc.operacao($('#vl_etapa').val(), $('#totalv').val(), '-');
        $('#rest_totalv').val( mascaraglobal("[###.]###,##", vlrRest) );
    }

    function calculaTotal(msgDet){
    	msgDet                      = (msgDet == false ? false : true);
        var vlrEtapa             = $('#vl_etapa').val();
        var percentTotal, somaDet   = new Number();
        var idDetalhamento          = '';
        var vlrTotal                = new String('0');
        $('[name*=ditvalor]').not(':last')
                             .each( function (i, objItem){
            somaDet     = 0;
            var vlrItem = $(objItem).val();
            if($(objItem).parent().parent().attr('id')){
                 //filho
                idDetalhamento = $(objItem).parent().parent().attr('idPai');

                $('#' + idDetalhamento).find('tr:not(:first)').each(function (a, objSub){
                    if ($(objSub).attr('id') != $(objItem).parent().parent().attr('id')){
                        somaDet = obCalc.operacao(somaDet, $(objSub).find('td:eq(7) input').val(), '+');
                    }
                });
                somaDet = obCalc.operacao(somaDet, vlrItem, '+');

                if ( obCalc.comparar(somaDet, $('#' + idDetalhamento).find('td:eq(7) input').val(), '>') ){
//                    if (confirm('A soma dos subitens excedem o valor do item de detalhamento!\nDeseja aumentar o item de detalhamento e caso seja necessário todos os itens superiores no cronograma da obra?')){
//                        $('#' + idDetalhamento).find('td:eq(7) input')
//                                               .val( formatarNumeroMonetario( {value : new Number(somaDet).toFixed(2)} ) )
//                                               .css({color:'red'});
//                        calculaTotal( false );
//                        return;
//                    }else{
						alert('A soma dos subitens excedem o valor do item de detalhamento!');
                        itemEdicao.value = vlrItemAnterior;
                        calculaTotal();
                        return;
//                    }
                }
                flgFilho = true;
            }else{
                //pai
                vlrTotal    = obCalc.operacao(vlrTotal, vlrItem, '+');
                vlrTotal    = mascaraglobal("[###.]###,##", vlrTotal);
                 // Calcula percent TOTAL
                percentTotal = obCalc.operacao(vlrTotal, vlrEtapa, '/', 20);
                percentTotal = obCalc.operacao(percentTotal, 100, '*', 20);
                percentTotal = formatarNumeroMonetario( {value : new Number(percentTotal).toFixed(2)} );
                $('#total').val( percentTotal );

                // Calcula resto do percent TOTAL
                var percentRest = obCalc.operacao('100', percentTotal, '-');
                percentRest     = formatarNumeroMonetario( {value:percentRest} );
                $('#rest_total').val( percentRest );

                flgFilho = false;
            }
            calculaPercent(objItem, i, flgFilho);
        });

        $('#totalv').val( vlrTotal );

        if ( obCalc.comparar(vlrTotal, $('#vl_etapa').val(), '>') && msgDet == true){
//        	if (confirm('A soma dos itens de detalhamento excedem o valor da etapa!\nDeseja aumentar a etapa e caso seja necessário todos os itens superiores no cronograma da obra?')){
//                $('#vl_etapa').val( vlrTotal )
//                              .css({color:'red'});
//
//                $('#vl_etapa_atualizado').val( vlrTotal );
//                $('#atualizarNivel').val( 1 );
//
//        	}else{
				alert('A soma dos itens de detalhamento excedem o valor da etapa!');
        		itemEdicao.value = vlrItemAnterior;
        		calculaTotal();
//        	}
        }else if (obCalc.comparar(vlrTotal, $('#vl_etapa').val(), '>') && msgDet == false){
//            $('#vl_etapa').val( vlrTotal )
//                          .css({color:'red'});
//
//            $('#vl_etapa_atualizado').val( vlrTotal );
//            $('#atualizarNivel').val( 1 );
        }


//        if ( obCalc.comparar(vlrTotal, $('#vl_etapa').val(), '>') ){
////            alert('A soma dos detalhamentos excede o valor da etapa.');
//            itemEdicao.value = vlrItemAnterior;
////            calculaTotal();
//        }


//        if ( obCalc.comparar(vlrTotal, $('#vl_estutura').val(), '>') ){
//
//            if (confirm('A soma das etapas excede o valor da estrutura.\nDeseja aumentar o valor da estrutura e em caso da soma das estruturas exceder o valor da obra aumentá-la também?')){
//                $('#vl_estutura').val( vlrTotal )
//                                 .css({color:'red'});
//                $('#vl_estutura_atualizado').val( vlrTotal );
//                $('#atualizarNivel').val( 1 );
//            }else{
//                itemEdicao.value = vlrItemAnterior;
//            }
//            calculaTotal();
//        }



    }

    function formatarNumeroMonetario( obj ){
        var valor = new String( obCalc.retiraZeroEsquerda(obj.value) );

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

}

calculoDetalhamento = new calculoDetalhamento();

<?php
$detalheitem = new DetalheItem();

$where=array("icoid = {$icoid}","ditidpai IS NULL");
$dadosDetalhe = $detalheitem->getDados($where);
$dadosDetalhe = (is_array( $dadosDetalhe ) ? $dadosDetalhe : array());

foreach ($dadosDetalhe as $dado){
	//adicionando o pai
    $dado['ditdatainicio']  = formata_data( $dado['ditdatainicio'] );
    $dado['ditdatatermino'] = formata_data( $dado['ditdatatermino'] );
    $dado['ditdsc'] = stripslashes($dado['ditdsc']);
    $dado['ditdsc'] = str_replace( '\\','', $dado['ditdsc'] );
    $dadoEncode = array_map('utf8_encode', $dado);

    echo 'gerenteDetalhamento.addLine( ' . simec_json_encode( $dadoEncode ) . ' );' . "\n";

    //adicionando os filhos
	$where = array("ditidpai = {$dado['ditid']}");
	$dadosDetalheFilho = $detalheitem->getDados($where);
	$dadosDetalheFilho = (is_array( $dadosDetalheFilho ) ? $dadosDetalheFilho : array());
	foreach ($dadosDetalheFilho as $dadoFilho){
	    $dadoFilho['ditdatainicio']  = formata_data( $dadoFilho['ditdatainicio'] );
	    $dadoFilho['ditdatatermino'] = formata_data( $dadoFilho['ditdatatermino'] );
        $dadoFilho['ditdsc'] = stripslashes( $dadoFilho['ditdsc'] );
        $dadoFilho['ditdsc'] = str_replace( '\\','', $dadoFilho['ditdsc'] );
        echo "console.log('".stripslashes($dadoFilho['ditdsc'])."');";
	    $dadoFilhoEncode = array_map('utf8_encode', $dadoFilho);
	    echo 'gerenteDetalhamento.addLine( ' . simec_json_encode( $dadoFilhoEncode ) . ',"' . $dadoFilho['ditidpai'] . '" );' . "\n";
	}

}
?>

function validaForm(){
    var obCalc = new Calculo();
	var result = true;
	var msg    = '';


    var flgDiferente = false;
    $('#lista_detalhamento').find('[id*=tr_detalhamento_]:not([id*=tr_detalhamento_filha])').each(function (i, obj){
        var valorPai    = obCalc.converteMonetario( $(obj).find('[name*=ditvalor]').val() ).toString();
        var valorFilhos = 0;

        if($(obj).find('[id*=tr_detalhamento_filha]').length > 0){
	    	$(obj).find('[id*=tr_detalhamento_filha]').each(function (y, objF){
	    		valorFilhos = valorFilhos + obCalc.converteMonetario( $(objF).find('[name*=ditvalor]').val() );
	    	});

	    	if( new Number(valorPai).toFixed(2) != new Number(valorFilhos).toFixed(2)){
	        	flgDiferente = true;
	        	return false;
	        }
        }
   });
    if ( flgDiferente ){
        msg += '* A soma dos valores dos filhos tem que ser igual ao valor do pai.\n';
        result = false;
    }

	var arValDsc = new Array();
	$('[name*=ditdsc]').not(':last')
		               .each(function (i, obj){
		            	   arValDsc.push( obj.value );
			           });
	if ( jQuery.inArray('', arValDsc ) >= 0 ){
	    msg += '* O campo descrição é de preenchimento obrigatório.\n';
	    result = false;
	}

	var arDtInicio = new Array();
	$('[name*=ditdatainicio]').not(':last')
	                   .each(function (i, obj){
	                	   arDtInicio.push( obj.value );
	                   });
	if ( jQuery.inArray('', arDtInicio ) >= 0 ){
	    msg += '* O campo Data de Início é de preenchimento obrigatório.\n';
	    result = false;
	}

	var arDtTermino = new Array();
	$('[name*=ditdatatermino]').not(':last')
	                   .each(function (i, obj){
	                	   arDtTermino.push( obj.value );
	                   });
	if ( jQuery.inArray('', arDtTermino ) >= 0 ){
	    msg += '* O campo Data de Término é de preenchimento obrigatório.\n';
	    result = false;
	}

    var arValUmc  = new Array();
    var arValMeta = new Array();
    $('[name*=umcid]').not(':last')
                      .each(function (i, obj){
						  if ( jQuery.trim( obj.value ) != '' && jQuery.trim( $('[name*=ditmetafisica]:eq(' + i + ')').val() ) == '' ){
							msg += '* No item de detalhamento que a unidade de medida for preenchida a meta física também deve ser preenchida.\n';
							result = false;
							return;
						  }
						  
						  if ( jQuery.trim( $('[name*=ditmetafisica]:eq(' + i + ')').val() ) != '' && jQuery.trim( obj.value ) == '' ){
							msg += '* No item de detalhamento que a meta física for preenchida a unidade de medida também deve ser preenchida.\n';
							result = false;
							return;
						  } 
                      
//                    	  arValUmc.push( obj.value );
//                    	  arValMeta.push( $('[name*=ditmetafisica]:eq(' + i + ')').val() );
                      });

//    if ( jQuery.inArray('', arValUmc ) >= 0 && jQuery.inArray('', arValMeta ) == -1 ){
//        msg += '* No item de detalhamento que a meta física for preenchida a unidade de medida também deve ser preenchida.\n';
//        result = false;
//    }
//    if ( jQuery.inArray('', arValMeta ) >= 0 && jQuery.inArray('', arValUmc ) == -1 ){
//        msg += '* No item de detalhamento que a unidade de medida for preenchida a meta física também deve ser preenchida.\n';
//        result = false;
//    }
    
//	var arVlItem = new Array();
//	$('[name*=ditvalor]').not(':last')
//	                   .each(function (i, obj){
//	                	   arVlItem.push( obj.value );
//	                   });
//	if ( jQuery.inArray('0,00', arVlItem ) >= 0 || jQuery.inArray('', arVlItem ) >= 0 ){
//	    msg += '* O campo Valor do Item não pode ser igual a zero.\n';
//	    result = false;
//	}

//    if ( obCalc.converteMonetario($('#rest_totalv').val()) != 0 ){
//        msg += '* O campo Valor Restante tem que ser igual a zero.\n';
//        result = false;
//    }

    $('[name*=ditdatainicio]').not(':last').each(function (i, obj){
        var id = new String($(obj).attr('id')).split('_');
        id     = id[1];

        var dtInicio = new String($(obj).val()).split('/');
        dtInicio     = new Date(dtInicio[2], new Number(dtInicio[1])-1, dtInicio[0]);

        var dtTermino = new String($('#ditdatatermino_'+id).val()).split('/');
        dtTermino     = new Date(dtTermino[2], new Number(dtTermino[1])-1, dtTermino[0]);

        if(dtInicio > dtTermino ){
            msg += '* Nenhuma Data de Início pode ser maior que a Data de Término.\n';
            result = false;
            return false;
        }
    });
	if ( msg != '' ){
	    alert('Validação do formulário:\n\n' + msg);
	}else{
		// remove a matrix (linha do form de edificação) que fica invisível na tela
		$('#tr_matrix').remove();
		$('#tr_matrix_filha').remove();
		$('#evt').val('salvar');
		$('#formDetalhamento').submit();
    }
}

</script>
