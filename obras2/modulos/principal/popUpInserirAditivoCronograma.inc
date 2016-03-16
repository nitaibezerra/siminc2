<?
ini_set("memory_limit", "64M");
header('Content-type: text/html; charset="iso-8859-1"',true);

if( possui_perfil(PFLCOD_CALL_CENTER)){
    $desabilita = 'S';
}

$_SESSION['obras2']['crtid'] = (isset($_GET['crtid'])) ? $_GET['crtid'] : $_SESSION['obras2']['crtid'];
$contrato = new Contrato( $_SESSION['obras2']['crtid'] );
extract( $contrato->getDados() );

unset($ttaid);

if( empty($crtid) ) {
	die("<script>
			 alert('Contrato não encontrado.');
			 window.close();
		 </script>");
}

if ( $_POST['operacao'] == 'salvar' && $_POST['evt'] == 'salvar' ){
    salvaAditivo();
    $respCronograma = salvaCronograma();
    if($respCronograma){
        die("<script>
                alert('Operação realizada com sucesso!');
                window.opener.location.reload();
                window.close();
             </script>");
    }else{
        die("<script>
                alert('Não foi possível realizar a operação! Erro ao cadastrar os dados do Cronograma.');
                window.opener.location.reload();
                window.close();
             </script>");
    }
}

if ($_REQUEST['tipoTela'] == 4){
    renderizaCronograma();
    exit;
} elseif ($_POST['tipoTela'] == 1){
?>
	<table class="Tabela" align="center" style="border: 0px; width:100%;">
	  <tr>
	    <th class="SubTituloDireita">Denominação:</th>
	    <td>
		<?= campo_texto( 'tradsc', 'S', 'S', '', 50, 60, '', '', 'left', '', 0); ?>
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Data de Assinatura do Aditivo:</th>
	    <td>
		<?= campo_data2( 'tradtassinatura', 'S', 'S', '', 'S', '', 'valDtAssinaturaContrato();' ); ?>
		</td>
	  </tr>
        <tr>
            <th class="SubTituloDireita">Data de Término da Vigência do Aditivo do Contrato:</th>
            <td>
                <?= campo_data2( 'traterminovigencia', 'S', 'S', '', 'S', '', 'dtVigencia();', null,  'dtVigencia();'); ?>
            </td>
        </tr>
        <tr>
            <th class="SubTituloDireita">Prazo de Vigência do Aditivo do Contrato (dias):</th>
            <td>
                <?= campo_texto( 'traprazovigencia', 'S', 'N', '', 5, 10, '[#]', '', 'left', '', 0); ?>
            </td>
        </tr>
	  <tr>
	    <th class="SubTituloDireita">Justificativa:</th>
	    <td>
		<?=campo_textarea( 'trajustificativa', 'S', 'S', '', '70', '8', 2000); ?>
		</td>
	  </tr>
	  <tr>
		<th class="SubTituloDireita">Anexo:</th>
		<td>
			<input type="file" name="arquivo" id="arquivo"/>
		</td>
	  </tr>
	</table>
<?
die;
}
elseif ($_POST['tipoTela'] == 2){
?>
	<table class="Tabela" align="center" style="border: 0px; width:100%;">
	  <tr>
	    <th class="SubTituloDireita">Denominação:</th>
	    <td>
		<?= campo_texto( 'tradsc', 'S', 'S', '', 50, 60, '', '', 'left', '', 0); ?>
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Data de Assinatura do Aditivo:</th>
	    <td>
		<?= campo_data2( 'tradtassinatura', 'S', 'S', '', 'S', '', 'valDtAssinaturaContrato();' ); ?>
		</td>
	  </tr>
        <tr>
            <th class="SubTituloDireita">Valor Final do Contrato Incluindo Aditivo(R$):</th>
            <td>
                <?=campo_texto( 'travlrfinalobra', 'S', 'S', '', 20, 11, '[###.]###,##', '', 'left', '', 0,  'onchange="calculoValorFinal();"', 'calculoValorFinal();' ); ?>
            </td>
        <tr>

        <tr>
            <th class="SubTituloDireita">Aditivo de Supressão:</th>
            <td>
                <ul style="margin: 0; padding: 0;">
                    <li style="margin:0;width:80px; list-style-type: none; float:left; ">
                        <input class="disabled" disabled="disabled" type="radio" name="trasupressao" value="S" id="radio_opcao_trasupressao_S">
                        <label for="radio_opcao_trasupressao_S">Sim</label>
                    </li>
                    <li style="margin:0;width:80px; list-style-type: none; float:left; ">
                        <input class="disabled" disabled="disabled" type="radio" name="trasupressao" value="N" id="radio_opcao_trasupressao_N">
                        <label for="radio_opcao_trasupressao_N">Não</label>
                    </li>
                </ul>
            </td>
        </tr>
        <script type="text/javascript">
            $(function(){
                $('[name="trasupressao"]').click(function(){return false;});
            });
        </script>
        <tr>
            <th class="SubTituloDireita">Valor do Aditivo(R$):</th>
            <td>
                <?= campo_texto( 'travlraditivo', 'S', 'N', '', 20, 11, '[###.]###,##', '', 'left', '', 0 ); ?>
            </td>
        </tr>
	  <tr>
	  <tr>
	    <th class="SubTituloDireita">Justificativa:</th>
	    <td>
		<?=campo_textarea( 'trajustificativa', 'S', 'S', '', '70', '8', 2000); ?>
		</td>
	  </tr>
	  <tr>
		<th class="SubTituloDireita">Arquivo:</th>
		<td>
			<input type="file" name="arquivo" id="arquivo"/>
		</td>
	  </tr>
        <tr>
            <th class="SubTituloDireita">Planilha de Custos:</th>
            <td>
                <input type="file" name="arqidcusto" id="arqidcusto"/>
            </td>
        </tr>

	</table>
<?
die;
}elseif ($_POST['tipoTela'] == 3){
?>
	<table class="Tabela" align="center" style="border: 0px; width:100%;">
	  <tr>
	    <th class="SubTituloDireita">Denominação:</th>
	    <td>
		<?= campo_texto( 'tradsc', 'S', 'S', '', 50, 60, '', '', 'left', '', 0); ?>
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Data de Assinatura do Aditivo:</th>
	    <td>
		<?= campo_data2( 'tradtassinatura', 'S', 'S', '', 'S', '', 'valDtAssinaturaContrato();' ); ?>
		</td>
	  </tr>
        <tr>
            <th class="SubTituloDireita">Data de Término da Vigência do Aditivo do Contrato:</th>
            <td>
                <?= campo_data2( 'traterminovigencia', 'S', 'S', '', 'S', '', 'dtVigencia();', null,  'dtVigencia();'); ?>
            </td>
        </tr>
        <tr>
            <th class="SubTituloDireita">Prazo de Vigência do Aditivo do Contrato (dias):</th>
            <td>
                <?= campo_texto( 'traprazovigencia', 'S', 'N', '', 5, 10, '[#]', '', 'left', '', 0); ?>
            </td>
        </tr>

        <tr>
            <th class="SubTituloDireita">Valor Final do Contrato Incluindo Aditivo(R$):</th>
            <td>
                <?=campo_texto( 'travlrfinalobra', 'S', 'S', '', 20, 11, '[###.]###,##', '', 'left', '', 0,  'onchange="calculoValorFinal();"', 'calculoValorFinal();' ); ?>
            </td>
        <tr>

        <tr>
            <th class="SubTituloDireita">Aditivo de Supressão:</th>
            <td>
                <ul style="margin: 0; padding: 0;">
                    <li style="margin:0;width:80px; list-style-type: none; float:left; ">
                        <input class="disabled" disabled="disabled" type="radio" name="trasupressao" value="S" id="radio_opcao_trasupressao_S">
                        <label for="radio_opcao_trasupressao_S">Sim</label>
                    </li>
                    <li style="margin:0;width:80px; list-style-type: none; float:left; ">
                        <input class="disabled" disabled="disabled" type="radio" name="trasupressao" value="N" id="radio_opcao_trasupressao_N">
                        <label for="radio_opcao_trasupressao_N">Não</label>
                    </li>
                </ul>
            </td>
        </tr>
        <script type="text/javascript">
            $(function(){
                $('[name="trasupressao"]').click(function(){return false;});
            });
        </script>
        <tr>
            <th class="SubTituloDireita">Valor do Aditivo(R$):</th>
            <td>
                <?= campo_texto( 'travlraditivo', 'S', 'N', '', 20, 11, '[###.]###,##', '', 'left', '', 0 ); ?>
            </td>
        </tr>

	  <tr>
	    <th class="SubTituloDireita">Justificativa:</th>
	    <td>
		<?=campo_textarea( 'trajustificativa', 'S', 'S', '', '70', '8', 2000); ?>
		</td>
	  </tr>
	  <tr>
		<th class="SubTituloDireita">Arquivo:</th>
		<td>
			<input type="file" name="arquivo" id="arquivo"/>
		</td>
	  </tr>
        <tr>
            <th class="SubTituloDireita">Planilha de Custos:</th>
            <td>
                <input type="file" name="arqidcusto" id="arqidcusto"/>
            </td>
        </tr>
	</table>
<?
die;
}
?>
<html>
<head>
	<title>Inserir Aditivo</title>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
</head>
<body topmargin="0" leftmargin="0" onload="abreAditivo('<?=$ttaid ?>');">
<form method="post" action="" name="form_aditivo" id="form_aditivo" enctype="multipart/form-data">
<table class="Tabela aditivo" align="center">
  <tr>
    <th class="SubTituloDireita">Tipo de Aditivo:</th>
    <td>
    <?
	$sql = "SELECT
				ttaid AS codigo,
				ttadsc AS descricao
			FROM
				obras2.tipotermoaditivo;";

	$db->monta_combo("ttaid", $sql, 'S', "Selecione...", "abreAditivo", '', '', '', 'S', 'ttaid');
	?>
    <input type="hidden" name="operacao" value="salvar">
	<input type="hidden" name="crtdtassinatura" value="<?=formata_data( $crtdtassinatura ) ?>">
	<input type="hidden" name="crtdttermino" value="<?=formata_data( $crtdttermino )?>">
	<input type="hidden" name="crtprazovigencia" value="<?=$crtprazovigencia?>">
	<input type="hidden" name="crtvalorexecucao" value="<?=$crtvalorexecucao ?>">
    </td>
  </tr>
  <tr style="display:none;" id="trTelaAditivo">
  	<td id="telaAditivo" colspan="2"></td>
  </tr>
  <tr id="trTelaAditivoObrasVinc">
  	<td id="telaAditivoObrasVinc" colspan="2" style="">
  		<fieldset style="display:none">
  			<legend>Obras Vinculadas ao Aditivo</legend>
	  		<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" style="margin-bottom: 10px;" class="listagem">
	  		<tr bgcolor="#F5F5F5">
	  			<td class="SubTituloCentro">&nbsp;</td>
	  			<td class="SubTituloCentro">Obra</td>
	  			<td class="SubTituloCentro">Início Anterior</td>
	  			<td class="SubTituloCentro">Prazo Anterior</td>
	  			<td class="SubTituloCentro">Término Anterior</td>
	  			<td class="SubTituloCentro">Valor Anterior</td>
	  			<td class="SubTituloCentro" id="td_tit_prz1">Acréscimo de Prazo</td>
	  			<td class="SubTituloCentro" id="td_tit_prz2">Novo Prazo</td>
	  			<td class="SubTituloCentro" id="td_tit_prz3">Novo Término</td>
	  			<td class="SubTituloCentro" id="td_tit_vlr1">Acréscimo de valor</td>
	  			<td class="SubTituloCentro" id="td_tit_vlr2">Novo Valor</td>
	  		</tr>
  	<?php
  	$obraContrato  = new ObrasContrato();
  	$arDadosContrato = $obraContrato->listaByContrato( $crtid, null, array() );
  	$i=0;
  	foreach ( $arDadosContrato as $dadosContrato ){
  		$background = ($i%2 == 0 ? '#FFFFFF' : '#F5F5F5');
  		$i++;
  	?>
	  		<tr bgcolor="<?=$background?>">
	  			<td align="center" width="10">
                    <? if(count($arDadosContrato) == 1): ?>
                        <input name="obrid[]" type="checkbox" id="obrid_<?=$dadosContrato['obrid'] ?>" value="<?=$dadosContrato['obrid'] ?>" checked="checked" disabled="disabled">
                    <? else: ?>
                        <input name="obrid[]" type="checkbox" onclick="habilitarCamposObra(this);" id="obrid_<?=$dadosContrato['obrid'] ?>" value="<?=$dadosContrato['obrid'] ?>">
                    <? endif; ?>
	  			</td>
	  			<td>

	  				<input type="hidden" name="ocrqtdconstrucao" id="ocrqtdconstrucao_<?=$dadosContrato['obrid'] ?>" value="<?=$dadosContrato['ocrqtdconstrucao'] ?>">
	  				<input type="hidden" name="ocrcustounitario" id="ocrcustounitario_<?=$dadosContrato['obrid'] ?>" value="<?=$dadosContrato['ocrcustounitario'] ?>">
	  				<input type="hidden" name="ocrnovocustounitario[<?=$dadosContrato['obrid'] ?>]" id="ocrnovocustounitario_<?=$dadosContrato['obrid'] ?>" value="">

	  				<label for="obrid_<?=$dadosContrato['obrid'] ?>" style="cursor:pointer;">
	  				<?=$dadosContrato['obrnome']?>
	  				</label>
	  			</td>
	  			<td>
	  				<?=campo_data2('ocrdtinicioexecucao', 'N', 'N','Início de Execução do Objeto', '', '', '', formata_data($dadosContrato['ocrdtinicioexecucao']), '', '', 'ocrdtinicioexecucao_' . $dadosContrato['obrid']); ?>
	  			</td>
	  			<td>
	  				<?=campo_texto('ocrprazoexecucao', 'N', 'N', '', 5, 10, '[#]', '', 'right', '', 0, 'id="ocrprazoexecucao_'.$dadosContrato['obrid'].'" onchange=""','', $dadosContrato['ocrprazoexecucao'], '' ); ?>
	  			</td>
	  			<td>
	  				<?=campo_data2('ocrdtterminoexecucao', 'N', 'N','Término de Execução do Objeto', '', '', '', formata_data($dadosContrato['ocrdtterminoexecucao']), '', '', 'ocrdtterminoexecucao_'.$dadosContrato['obrid']); ?>
	  			</td>
	  			<td>
					<?=campo_texto('ocrvalorexecucao', 'N', 'N', '', 12, 18, '[###.]###,##', '', 'right', '', 0, 'id="ocrvalorexecucao_'.$dadosContrato['obrid'].'"','', number_format( $dadosContrato['ocrvalorexecucao'], 2, ',', '.'), '' ); ?>
				</td>
	  			<td class="td_prazo_exec">
	  				<?=campo_texto('acrescimoprazoexecucao', 'S', 'S', '', 5, 10, '[#]', '', 'right', '', 0, 'readonly id="acrescimoprazoexecucao_'.$dadosContrato['obrid'].'" onchange="dataTerminoObraAditivo(this);"','dataTerminoObraAditivo(this);', '', '' ); ?>
	  			</td>
	  			<td class="td_prazo_exec">
	  				<?=campo_texto('ocrnovoprazoexecucao['.$dadosContrato['obrid'].']', 'N', 'N', '', 5, 10, '[#]', '', 'right', '', 0, 'id="ocrnovoprazoexecucao_'.$dadosContrato['obrid'].'"', '', '', '' ); ?>
	  			</td>
	  			<td class="td_prazo_exec">
	  				<?=campo_data2('ocrnovodtterminoexecucao['.$dadosContrato['obrid'].']', 'N', 'N','Término de Execução do Objeto', '', '', '', '', '', 'ocrnovodtterminoexecucao', 'ocrnovodtterminoexecucao_'.$dadosContrato['obrid']); ?>
	  			</td>
	  			<td class="td_valor_exec">
					<?=campo_texto('acrescimovalorexecucao', 'S', 'S', '', 12, 18, '[###.]###,##', '', 'right', '', 0, 'readonly id="acrescimovalorexecucao_'.$dadosContrato['obrid'].'" onBlur="preencheValorAditivo(this);"','preencheValorAditivo(this);', '' ); ?>
				</td>
	  			<td class="td_valor_exec">
					<?=campo_texto('ocrnovovalorexecucao['.$dadosContrato['obrid'].']', 'N', 'N', '', 12, 18, '[###.]###,##', '', 'right', '', 0, 'id="ocrnovovalorexecucao_'.$dadosContrato['obrid'].'"','', '' ); ?>
				</td>
	  		</tr>
  	<?php
  	}
  	?>
  			</table>
  		</fieldset>
  	</td>
  </tr>
  <tr id="trTelaCronograma">
      <td><input type="button" value="Exibir Cronograma" onclick="editarContrato();"></td>
  </tr>
  <tr style="display: table-row;">
      <td colspan="2"><fieldset id="contrato" style="display:none">
            <legend>Contrato</legend>
            <div id="contrato-conteiner">
            </div>
  		</fieldset>	</td>
  </tr>
  <tr style="display:none;" id="trButton" bgcolor="#C0C0C0">
  	<td colspan="2" align="center">

  		<input type="button" value="Salvar" onclick="validaForm(); //valTerminoVigenciaExecucao();">
  	</td>
  </tr>
</table>

<script type="text/javascript">
function abreAditivo(value){
	if (value){
		divCarregando();
		var traid = '<?=$_REQUEST['traid'] ?>';
		var dado = {async: false, "tipoTela" : value, "traid" : traid};
		$('#telaAditivo').load("?modulo=principal/popUpInserirAditivo&acao=A", dado, function (){
					if(traid){atualizaParaEdicao();}
					divCarregado();
				});

		if( value==1 ) {

			$("[name*=acrescimovalorexecucao]").each(function (){
				$(this).val("0");
				$(this).keyup();
				$('.td_valor_exec').hide();
				$('#td_tit_vlr1').hide();
				$('#td_tit_vlr2').hide();

				$('.td_prazo_exec').show();
				$('#td_tit_prz1').show();
				$('#td_tit_prz2').show();
				$('#td_tit_prz3').show();


			});
		}else if( value==2 ) {
			$("[name*=acrescimoprazoexecucao]").each(function (){
				$(this).val("0");
				$(this).keyup();
				$('.td_prazo_exec').hide();
				$('#td_tit_prz1').hide();
				$('#td_tit_prz2').hide();
				$('#td_tit_prz3').hide();

				$('.td_valor_exec').show();
				$('#td_tit_vlr1').show();
				$('#td_tit_vlr2').show();

			});

		}else if ( value==3 ){
			$("[name*=acrescimoprazoexecucao]").each(function (){
				$(this).val("0");
				$(this).keyup();
				$('.td_prazo_exec').show();
				$('#td_tit_prz1').show();
				$('#td_tit_prz2').show();
				$('#td_tit_prz3').show();

				$('.td_valor_exec').show();
				$('#td_tit_vlr1').show();
				$('#td_tit_vlr2').show();
			});
		}
		//
		$('#trTelaAditivo').show();
		$('#trTelaAditivoObrasVinc').show();
		$('#trTelaCronograma').show();
		$('#trButton').show();
	}else{
		$('#telaAditivo').html("");
		$('#trTelaAditivo').hide();
		$('#trTelaAditivoObrasVinc').hide();
		$('#trTelaCronograma').hide();
		$('#trButton').hide();
	}
        <?php
            if ($desabilita == 'S') {?>
            $("#ttaid" ).click(function() {
                $(document).ready(function (){
                    setTimeout(function(){ 
                        jQuery('#form_aditivo').find(':input').attr('disabled',true); 
                    }, 1500);
                });
            });
    <?php }?>
}


/*
 *	Funções utilizadas no aditivo de PRAZO
 */
function dtVigencia(){

	var dt   = $("[name='crtdttermino']").val();
	var dtFim  = $("[name='traterminovigencia']").val();
    var date1 = new Date( dtFim.substr(6,4) + '-' + dtFim.substr(3,2) + '-' + dtFim.substr(0,2) );
    var date2 = new Date( dt.substr(6,4) + '-' + dt.substr(3,2) + '-' + dt.substr(0,2) );
    var diffDays = parseInt((date1 - date2) / (1000 * 60 * 60 * 24));

	if ( dt && diffDays ){
        if(diffDays <= 0){
            alert('O campo "Data de Término da Vigência do Aditivo do Contrato" deve ser maior que a data de termino do contrato atual.');
            $("[name='traprazovigencia']").val('');
            $("[name='traterminovigencia']").val('');
        } else {
            $("[name='traprazovigencia']").val(diffDays);
        }
	}else{
		if ( !dt ){
			alert('O "Data de término do contrato (aba contratação)", não foi informado!\nInforme-o antes de incluir o aditivo.');
		}
		$("[name='traprazovigencia']").val('');
        $("[name='traterminovigencia']").val('');
	}

    $("[name='acrescimoprazoexecucao']:eq(0)").val($("[name='traprazovigencia']").val()).change();
}


function valDtAssinaturaContrato(){
	var msg;
	var dtAssinaturaAditivo = $("#tradtassinatura").val();
	var dtAssinaturaContrat = $('[name="crtdtassinatura"]').val();
	var diaVigenciaContrat	= $('[name="crtprazovigencia"]').val();

	if ( dtAssinaturaAditivo && dtAssinaturaContrat && diaVigenciaContrat ){
		var obDt = new Data();
		var dtTerminoVigenciaContrat = obDt.dtAddDia(dtAssinaturaContrat, diaVigenciaContrat);

		if ( !obDt.comparaData(dtAssinaturaAditivo, dtTerminoVigenciaContrat, '<=') ){
			alert('A "Data de assinatura do aditivo" deve ser MENOR ou IGUAL ao "Término da vigência do contrato ( '+dtTerminoVigenciaContrat+' - aba contratação)"');
			$("#tradtassinatura").val("");
			return false;
		}

		if ( !obDt.comparaData(dtAssinaturaAditivo, dtAssinaturaContrat, '>') ){
			alert('A "Data de assinatura do aditivo" deve ser MAIOR do que a "Data de Assinatura do Contrato ( '+dtAssinaturaContrat+' - aba contratação)"');
			$("#tradtassinatura").val("");
			return false;
		}

	}else{
		if ( dtAssinaturaContrat == "" ){
			msg = 'A "Data de Assinatura do Contrato (aba contratação)" não foi informada!\nInforme-a antes de incluir o aditivo.\n';
		}
		if ( diaVigenciaContrat == "" ){
			msg += 'O "Prazo de Vigência do Contrato (aba contratação)" não foi informado!\nInforme-o antes de incluir o aditivo.\n';
		}
		if (msg != ""){
			alert( msg );
			$("#tradtassinatura").val("");
			return false;
		}
	}


	return true;
}

function valPrazoVigenciaExecucao(){
	var prazoVigencia = $("[name='traterminovigencia']").val() ;
	var prazoExecucao = $("[name='traterminoexec']").val() ;

	if ( prazoVigencia && prazoExecucao ){
		var obDt = new Data();
		if ( obDt.comparaData(prazoVigencia, prazoExecucao, '<') ){
			alert('A "Data de Término da Vigência do Aditivo do Contrato" deve ser MAIOR ou IGUAL ao "Término da Execução do Aditivo da Obra"!');
//			alert('O "Prazo de Vigência do Aditivo do Contrato" deve ser MAIOR ou IGUAL ao "Prazo do Aditivo de Execução da Obra"!');
			return false;
		}
	}
	return true;
}

function valPrazoObra() {
	var dtTerminoVigencia = $("[name='traterminovigencia']").val();

	var invalidar = false;

	$("[name^='ocrnovodtterminoexecucao[']").each(function (){
		var dtNovoTerminoExecucao = $(this).val();

		var obDt = new Data();
		if ( !obDt.comparaData(dtTerminoVigencia, dtNovoTerminoExecucao, '>=') ){
			alert('O "Data de Término da Vigência do Aditivo do Contrato" deve ser MAIOR ou IGUAL ao "Data do Novo Término da Execução da Obra"!');
			invalidar = true;
		}

	});

	if(invalidar) {

		return false;

	} else {

		return true;

	}
}

function valTerminoVigenciaExecucao(){
	var dtTerminoVigencia = $("[name='traterminovigencia']").val();
	var dtTerminoExecucao = $("[name='traterminoexec']").val();

	if ( dtTerminoVigencia && dtTerminoExecucao ){
		var obDt = new Data();
		if ( !obDt.comparaData(dtTerminoVigencia, dtTerminoExecucao, '>=') ){
			alert('O "Data de Término da Vigência do Aditivo do Contrato" deve ser MAIOR ou IGUAL ao "Término da Execução do Aditivo da Obra"!');
			return false;
		}
	}
	return true;
}
/*
 *	FIM - Funções utilizadas no aditivo de PRAZO
 */

/*
 * Funções usadas no aditivo de VALOR
*/
function calculoValorFinal(){
	var vlrCustoObra = $("[name='crtvalorexecucao']").val();
	var vlrFinal	 = $("[name='travlrfinalobra']").val();

	if ( vlrCustoObra == "" ){
		alert('O "valor Contratado da Obra (aba contratação)" não foi informado!\nInforme-o antes de incluir o aditivo.');
		$("[name='travlraditivo']").val("");
		return false;
	}

    var obCal = new Calculo();
    var comp = obCal.comparar(vlrFinal, vlrCustoObra, '<');
    var operador = comp == true ? '+' : '-';
	var vlrAditivo = obCal.operacao(vlrCustoObra, vlrFinal, '-');

    $("[name='travlraditivo']").val( mascaraglobal('[###.]###,##', vlrAditivo ));

    if(operador == '+'){
        jQuery("[name='trasupressao']").eq(0).attr('checked', true);
    } else {
        jQuery("[name='trasupressao']").eq(1).attr('checked', true);
    }

    $("[name='acrescimovalorexecucao']").val(vlrAditivo).keyup();
}

/*
 * FIM -> Funções usadas no aditivo de VALOR
*/

function valDtTermino(){
	var retorno = true;
	if ($.trim($('[name="crtdttermino"]').val()) == ''){
		alert('A "Data de término do contrato (aba contratação)", não foi informado!\n');
		retorno = false;
	}

	return retorno;
}

function valObrasVinculadas(){
	var retorno = true;

	if ( $("[id*=obrid_]:checked").length == 0 ){
		alert('É necessário informar a(s) obra(s) que será(ão) vinculada(s) ao aditivo.');
		retorno = false;
	}

	return retorno;
}

function editarContrato(){
    if(validaForm(true)){
        divCarregando();
        var dado = {async: false, "tipoTela" : 4};
        $.ajax({
            type: "POST",
            url: '?modulo=principal/popUpInserirAditivoCronograma&acao=A',
            data: dado,
            success: function (data){
                $('#contrato').show();
                $('#contrato #contrato-conteiner').html(data);

                if($('select[name=ttaid]').val() == 1){
                    $('#crtdatafimLabel').html($('.td_prazo_exec:eq(2) input').val());
                }

                if($('select[name=ttaid]').val() == 2){
                    $('#crtvalorLabel').html($('.td_valor_exec:eq(1) input').val());
                    $('#vl_contrato').val($('.td_valor_exec:eq(1) input').val());
                }

                if($('select[name=ttaid]').val() == 3){
                    $('#crtvalorLabel').html($('.td_valor_exec:eq(1) input').val());
                    $('#vl_contrato').val($('.td_valor_exec:eq(1) input').val());
                    $('#crtdatafimLabel').html($('.td_prazo_exec:eq(2) input').val());
                }

                calculoEdificacao.manager();

                divCarregado();
            }
        });
    }
}

function validaForm(valid){
	try{
		$("#trTelaAditivo :text, #telaAditivoObrasVinc :text").each(function (){
			if ( $(this).val() == "" ){
				alert("Preencha todos os campos obrigatórios!");
				$(this).focus();
				$(this).select();
				throw "Erro: Campo Obrigatório.";
				return false;
			}
		});

		$("#trTelaAditivo textarea, #telaAditivoObrasVinc textarea").each(function (){
			if ( $(this).val() == "" ){
				alert("Preencha todos os campos obrigatórios!");
				$(this).focus();
				$(this).select();
				throw "Erro: Campo Obrigatório.";
			}
		});

		if($("#arquivo").val()=='') {
			alert('Arquivo é obrigatório');
			return false;
		}

		if ( !valDtAssinaturaContrato() ) throw "Erro: valDtAssinaturaContrato();";
		if ( !valPrazoVigenciaExecucao() ) throw "Erro: valPrazoVigenciaExecucao();";
		if ( !valTerminoVigenciaExecucao() ) throw "Erro: valTerminoVigenciaExecucao();";
		if ( !valDtTermino() ) throw "Erro: valDtTermino();";
		if ( !valObrasVinculadas() ) throw "Erro: valObrasVinculadas();";
		if(jQuery("#traterminovigencia").length) {
			if ( !valPrazoObra() ) throw "Erro: valPrazoObra();";
		}

		if(jQuery("#traterminovigencia").length) {
			if ( !valPrazoObra() ) throw "Erro: valPrazoObra();";
		}

        if(valid === true)
            return true;

       if($('#contrato #contrato-conteiner').html().trim() == "") {
           alert("O cronograma ainda não foi revisado.\n\nUtilize o botão 'Exibir cronograma' para revisá-lo");
           return false;
       } else {
            if($('#totalv').val() != $('#vl_contrato').val()){
                alert("A soma do valor das etapas deve ser igual ao valor do contrato.");
                return false;
            }
       }

        if(!$('#contrato #contrato-conteiner').is(':empty')){
            if(!validaFormContrato()){
//                alert("O cronograma ainda não foi revisado.\n\nUtilize o botão 'Exibir cronograma' para revisalo.");
                return false;
            }
        }
        <?
            $obrid 						 = $_SESSION['obras2']['obrid'];
            $obra = new Obras($obrid);
        ?>
        if(confirm("Atenção! Você esta prestes a confirmar a inserção de um aditivo para um contrato registrado no sistema.\nTodas as operações são armazenadas para auditoria.\n\nObra: <?=$obra->obrnome?>\n\nConfirmar a inserção do aditivo?")){
            $('#form_aditivo').submit();
        }

	}catch(err){
		return false;
	}

}

function habilitarCamposObra(obj) {

	if(obj.checked) {

		jQuery('#acrescimoprazoexecucao_'+obj.value).attr("className","obrigatorio normal");
		jQuery('#acrescimovalorexecucao_'+obj.value).attr("className","obrigatorio normal");

		jQuery('#acrescimoprazoexecucao_'+obj.value).attr("readonly","");
		jQuery('#acrescimovalorexecucao_'+obj.value).attr("readonly","");

	} else {

		jQuery('#acrescimoprazoexecucao_'+obj.value).attr("className","disabled");
		jQuery('#acrescimovalorexecucao_'+obj.value).attr("className","disabled");

		jQuery('#acrescimoprazoexecucao_'+obj.value).attr("readonly","readonly");
		jQuery('#acrescimovalorexecucao_'+obj.value).attr("readonly","readonly");

	}

}

function dataTerminoObraAditivo(obj){
	var table = jQuery( obj ).parents('tr')[0];

	var prExecucao = parseInt(jQuery( table ).find('[id*=ocrprazoexecucao]').val());
	var acrescPraz = parseInt(jQuery( table ).find('[id*=acrescimoprazoexecucao]').val());

	jQuery( table ).find('[id*=ocrnovoprazoexecucao]').val(prExecucao+acrescPraz);

	var dtInicio  = jQuery( table ).find('[id*=ocrdtinicioexecucao]').val();
	var diaPrazo  = jQuery( table ).find('[id*=ocrnovoprazoexecucao]').val();
	var dtTermino = '';

	if (dtInicio && diaPrazo){
		var obDt = new Data();
		dtTermino = obDt.dtAddDia(dtInicio, diaPrazo);
	}

	jQuery( table ).find('[id*=ocrnovodtterminoexecucao]').val( dtTermino );
    $('#crtdatafimLabel').html(dtTermino);
}


function preencheValorAditivo( obj ){
	var calc = new Calculo();
	var table = jQuery( obj ).parents('tr')[0];

	var vlrExecucaoAnt      = parseFloat(replaceAll(replaceAll(jQuery( table ).find('[id*=ocrvalorexecucao]').val(),'.',''),',','.'));
	var acrescVlrExecucao   = parseFloat(replaceAll(replaceAll(jQuery( table ).find('[id*=acrescimovalorexecucao]').val(),'.',''),',','.'));

	if(jQuery('#radio_opcao_trasupressao_N').attr('checked')) {
		var total = vlrExecucaoAnt+acrescVlrExecucao;
		jQuery( table ).find('[id*=ocrnovovalorexecucao]').val(mascaraglobal('###.###.###,##',total.toFixed(2)));
            $('#crtvalorLabel').html(mascaraglobal('###.###.###,##',total.toFixed(2)));
            $('#vl_contrato').val(mascaraglobal('###.###.###,##',total.toFixed(2)));
	} else {

		if(jQuery('#radio_opcao_trasupressao_S').attr('checked')) {
			var total = vlrExecucaoAnt-acrescVlrExecucao;
			jQuery( table ).find('[id*=ocrnovovalorexecucao]').val(mascaraglobal('###.###.###,##',total.toFixed(2)));
                $('#crtvalorLabel').html(mascaraglobal('###.###.###,##',total.toFixed(2)));
                $('#vl_contrato').val(mascaraglobal('###.###.###,##',total.toFixed(2)));
		} else {

			if ( jQuery("[name^='trasupressao']").length ){

//				alert('Marque a opção Aditivo de Supressão');
//				jQuery( table ).find('[id*=acrescimovalorexecucao]').val('');
//				return false;


			} else {

				jQuery( table ).find('[id*=ocrnovovalorexecucao]').val(mascaraglobal('###.###.###,##',vlrExecucaoAnt.toFixed(2)));
                    $('#crtvalorLabel').html(mascaraglobal('###.###.###,##',vlrExecucaoAnt.toFixed(2)));
                    $('#vl_contrato').val(mascaraglobal('###.###.###,##',vlrExecucaoAnt.toFixed(2)));
			}
		}

	}

	var vlrExecucao      = jQuery( table ).find('[id*=ocrnovovalorexecucao]').val();
	var qtdArea          = jQuery( table ).find('[id*=ocrqtdconstrucao]').val();
	var custoUnit        = jQuery( table ).find('[id*=ocrnovocustounitario]').val();
	var vlrCustoUnitario = '';

	if ( vlrExecucao != '' && qtdArea != '' ){
		vlrCustoUnitario = mascaraglobal('###.###.###,##', calc.operacao(vlrExecucao, qtdArea, '/') );
	}

	jQuery( table ).find('[id*=ocrnovocustounitario]').val(vlrCustoUnitario);

}

</script>
</body>
</html>








<?php

function renderizaCronograma(){

    // empreendimento || obra || orgao
    verificaSessao( 'obra' );

    $_SESSION['obras2']['obrid'] = ( $_GET['obrid'] ? $_GET['obrid'] : $_SESSION['obras2']['obrid'] );
    $obrid 						 = $_SESSION['obras2']['obrid'];

    function validaDataPHP( $data ){

        $data = new DateTime( $data );
    }

    $obraContrato = new ObrasContrato();

    if(obraMi($obrid)){
        $itensComposicao  = new ItensComposicaoObras();
        $ocrvalorexecucao = $itensComposicao->getValorTotalItens($obrid);
    } else {
        $ocrvalorexecucao = $obraContrato->getValorContrato($obrid);
    }

    $obra = new Obras( $obrid );
    $obrcronogramaservicocontratadojustificativa = $obra->obrcronogramaservicocontratadojustificativa;
    $obrcronogramaservicocontratado 			 = $obra->obrcronogramaservicocontratado;

    $docid = pegaDocidObra( $obrid );

    $contrato = new Contrato();
    $crtid = $contrato->pegaCrtidPorObrid( $obrid );

    if ( $crtid ){
        $habilitado = true;
        $habilita   = 'S';
    }

    monta_titulo( $titulo_modulo, '<b>Após inserir, atualizar ou excluir uma etapa, clique em salvar.</b>' );
    echo cabecalhoCronograma($obrid);
    ?>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <script type="text/javascript">

    jQuery(document).ready(function(){
        jQuery('#aguarde').hide();
    });

    function inserirEtapas(){
        return windowOpen( 'obras2.php?modulo=principal/inserir_etapas&acao=A','blank','height=450,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    function gerenteEdificacao(){

        var imgDesabUp = $( document.createElement('img') ).attr({src    : '/imagens/seta_cimad.gif',
                                                                  border : 0,
                                                                  title  : 'Subir'});

        var imgUp = $( document.createElement('img') ).attr({src    : '/imagens/seta_cima.gif',
                                                             border : 0,
                                                             title  : 'Subir'})
                                                      .css({cursor : 'pointer'});

        var imgDesabDown = $( document.createElement('img') ).attr({src    : '/imagens/seta_baixod.gif',
                                                                  border : 0,
                                                                  title  : 'Descer'});

        var imgDown = $( document.createElement('img') ).attr({src    : '/imagens/seta_baixo.gif',
                                                             border : 0,
                                                             title  : 'Descer'})
                                                        .css({cursor : 'pointer'});

        this.addLine = function (dataLine){
            dataLine = dataLine || '';

            if ( dataLine.itcid == '' ){
                alert('Falha!\nFaltam dados da etapa.');
                return false;
            }

            var idTR = 'tr_etapa_' + dataLine.itcid;

            var clone = $('#tr_matrix').clone()
                                       .attr('id', idTR)
                                       .show();

            clone.html( replaceAll (clone.html() , 'id="icodtinicioitem[]"', 'id="icodtinicioitem_' + dataLine.itcid + '"') );
            clone.html( replaceAll (clone.html() , "document.getElementById('icodtinicioitem[]')", "document.getElementById('icodtinicioitem_" + dataLine.itcid + "')") );

            clone.html( replaceAll (clone.html() , 'id="icodterminoitem[]"', 'id="icodterminoitem_' + dataLine.itcid + '"') );
            clone.html( replaceAll (clone.html() , "document.getElementById('icodterminoitem[]')", "document.getElementById('icodterminoitem_" + dataLine.itcid + "')") );

            prepareRemoveImg( clone, idTR, dataLine );

            $('#tr_matrix').before( clone );

            if ( dataLine != '' ){

    <?php
        if ( $habilitado ):
    ?>
                if ( dataLine.icoid ){
                    var img = $( document.createElement('img') ).attr({src    :    '/imagens/ico_config_i.gif',
                                                                       border : 0,
                                                                       title  : 'Ir para detalhamento da etapa'})
                                                                .css({cursor : 'pointer'})
                                                                .click(function (){
                                                                        location.href = '?modulo=principal/detalhe_da_etapa&acao=A&icoid=' + dataLine.icoid;
                                                                    });
                    clone.find('td')
                         .eq(1)
                         .find('img')
                         .before( img );
                    clone.find('td')
                         .eq(1)
                         .find('img')
                         .eq(1)
                         .before( '&nbsp;' )
                         .before( '&nbsp;' );
                }
    <?php
        endif;
    ?>
                loadData( dataLine, clone );
            }
    <?php
        if ( $habilitado ):
    ?>
            prepareArrowImg();
    <?php
        endif;
    ?>
            colorManager();
        }


        this.removeLine = function ( id ){
            $( '#' + id ).remove();
            colorManager();
            prepareArrowImg();

            calculoEdificacao.manager();
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

        function loadData( dataLine, line ){
            var hiddenID = $( document.createElement('input') ).attr({name : 'icoid[]',
                                                                      type : 'hidden'})
                                                               .val( (dataLine.icoid ? dataLine.icoid : '') );
            line.find('[id*=itcdesc]').html( dataLine.itcdesc )
                                      .after( hiddenID );

            line.find('[name*=itcid]').val( dataLine.itcid );
            line.find('[name*=icodtinicioitem]').val( dataLine.icodtinicioitem );
            line.find('[name*=icodterminoitem]').val( dataLine.icodterminoitem );
            line.find('[name*=icovlritem]').val( dataLine.icovlritem );

            calculoEdificacao.manager( line.find('[name*=icovlritem]')[0] );

        }

        function prepareArrowImg (){
            var indMaxTr = ($('#tabela_etapas').find('[id*=tr_etapa_]').length -1);
            $('#tabela_etapas').find('[id*=tr_etapa_]').each(function (i, obj){

                var cloneImgUp = (i == 0
                                    ? $( imgDesabUp ).clone()
                                    : $( imgUp ).clone()
                                                .bind('click', function (){
                                                    gerenteEdificacao.moveUp( obj );
                                                    }));
                var cloneImgDown = (i == indMaxTr
                                    ? $( imgDesabDown ).clone()
                                    : $( imgDown ).clone()
                                                  .bind('click', function (){
                                                    gerenteEdificacao.moveDown( obj );
                                                    }));

                $(obj).find('td:eq(0)').html('')
                                       .append( cloneImgUp )
                                       .append( '&nbsp;' )
                                       .append( cloneImgDown );
             });
        }

        function prepareRemoveImg( clone, idTR, dataLine ){
    <?php
        if ( $habilitado == false ):
    ?>
            $(clone).find('td:eq(1)').html('');
    <?php
        else:
    ?>
            dataLine.qtddetalhamento = dataLine.qtddetalhamento || 0;
            if ( typeof dataLine == 'string' || dataLine.qtddetalhamento == 0 ){
                $(clone).find('td:eq(1) span img').bind('click', function() {
                    if(confirm("Deseja remover esse registro?")) {
                        gerenteEdificacao.removeLine( idTR );
                    }
                });
            }else{
                $(clone).find('td:eq(1) span img')
                        .attr('src', '/imagens/excluir_01.gif')
                        .attr('title', 'A etapa não pode ser excluída, pois possui detalhamento(s).')
                        .bind('click', function() {
                            alert('A etapa não pode ser excluída, pois possui detalhamento(s).');
                        });
            }
    <?php
        endif;
    ?>
        }

        function colorManager (){
            var countLine = 0;
            var colorLine = new Array('#FFFFFF', '#E0E0E0');
            $('#tabela_etapas').find('[id*=tr_etapa_]').each(function (i, obj){
                   $( obj ).attr('bgcolor', colorLine[countLine]);
                   countLine = (countLine == 0 ? 1 : 0);
                });
        }
    }

    var gerenteEdificacao = new gerenteEdificacao();

    function calculoEdificacao(){
        var obCalc       = new Calculo();
        var percentTotal = new Number(0);
        var itemEdicao        = '';
        var vlrItemAnterior   = '';
    //	var vlrContrato  = obCalc.converteMonetario( $('#vl_contrato').val() );

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

        function calculaPercent(obj, ind){
            var vlrContrato = obCalc.converteMonetario( $('#vl_contrato').val() );
            var vlrItem     = obCalc.converteMonetario( obj.value );
            var percentItem = mascaraglobal("[###.]###,##", ((vlrItem / vlrContrato) * 100).toFixed(2) );
            percentItem     = formatarNumeroMonetario( {value:percentItem} );
            $('[name*=icopercsobreestrutura]').eq(ind)
                                      .val( percentItem );

        }

        function calculaRestante(){
            var vlrRest = obCalc.operacao($('#vl_contrato').val(), $('#totalv').val(), '-');
            $('#rest_totalv').val( mascaraglobal("[###.]###,##", vlrRest) );
        }

        function calculaTotal(){
            var vlrContrato = obCalc.converteMonetario( $('#vl_contrato').val() );
            var vlrTotal = new String('0');
            $('[name*=icovlritem]').not(':last')
                                 .each( function (i, objItem){
                    var vlrItem = $(objItem).val();
                    vlrTotal    = obCalc.operacao(vlrTotal, vlrItem, '+');
                    vlrTotal    = mascaraglobal("[###.]###,##", vlrTotal);

                    calculaPercent(objItem, i);
                });
            $('#totalv').val( vlrTotal );

            percentTotal = obCalc.operacao(vlrTotal, vlrContrato, '/', 20);
            percentTotal = obCalc.operacao(percentTotal, 100, '*', 20);
            percentTotal = new Number( percentTotal ).toFixed(2);
            $('#total').val( formatarNumeroMonetario( {value : percentTotal} ) );

            var percentRest = obCalc.operacao('100', percentTotal, '-');
            percentRest     = formatarNumeroMonetario( {value:percentRest} );
            $('#rest_total').val( percentRest );

            if ( obCalc.comparar(vlrTotal, $('#vl_contrato').val(), '>') ){
                        var pulaAlert;
                        if ( typeof itemEdicao != 'object' ){
                            pulaAlert  = true;
                            itemEdicao = new Object();
                            itemEdicao.value = 0;
                        }else{
                            pulaAlert = false;
                        }
                        vlrItemAnterior  = vlrItemAnterior || 0;
                        recalculaTotal = (itemEdicao.value != vlrItemAnterior ? true : false);

                        if ( recalculaTotal || pulaAlert ){
                            alert('A soma das etapas excede o valor do contrato.');
                            itemEdicao.value = vlrItemAnterior;
                        }
                if ( recalculaTotal ){
                    calculaTotal();
                }
            }

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

    calculoEdificacao = new calculoEdificacao();

    function verificaDataTermino(data){
        var obData = new Data();

        if( obData.comparaData(data.value,$('#crtdatainicioLabel').html(),'<') ){
            alert('A data de término não pode ser menor que a data de início do projeto.');
            data.value = '';
            data.focus();
            return;
        }

        if( obData.comparaData(data.value,$('#crtdatafimLabel').html(),'>') ){
            alert('A data de término não pode ser maior que a data de conclusão do projeto.');
            data.value = '';
            data.focus();
            return;
        }

        // verificando se a data de termino não é menor que a data de inicio
        var nome = ($(data).attr('id')).split('_');
        if(nome[1]){
            if( obData.comparaData(data.value,$('#icodtinicioitem_'+nome[1]).val(),'<') ){
                alert('A data de término não pode ser menor que a data de início.');
                data.value = '';
                data.focus();
                return;
            }
        }

    }

    </script>
    <input type="hidden" name="evt" id="evt" value="">
    <input type="hidden" name="vl_contrato_atualizado" id="vl_contrato_atualizado" value="0">
    <input type="hidden" name="atualizarNivel" 	id="atualizarNivel" value="0">
    <input type="hidden" name="dtInicioObra" 	id="dtInicioObra" 	value="">
    <input type="hidden" name="dtTerminoObra" 	id="dtTerminoObra" 	value="">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="SubTituloTelaCentro" colspan="2">Lista das Etapas da Obra</td>
        </tr>
        <tr>
            <td colspan="2">
                <table id="tabela_etapas" width="95%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
                    <thead>
                        <tr id="cabecalho">
                            <td width="5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ordem</strong></td>
                            <td width="5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
                            <td width="30%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Descrição</strong></td>
                            <td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data de Início</strong></td>
                            <td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data de Término</strong></td>
                            <td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor do Item (R$)</strong></td>
                            <td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>(%) Referente a Estrutura</strong></td>
                        </tr>
                    </thead>
                    <tr id="tr_matrix" style="display:none;">
                        <td align="center"></td>
                        <td align="center">
                            <span>
                                <img src='/imagens/excluir.gif' style='cursor:pointer;' border='0' title='Excluir'>
                            </span>
                        </td>
                        <td>
                            <span id="itcdesc[]"></span>
                            <input type="hidden" name="itcid[]" value="">
                        </td>
                        <td align="center"><?php echo campo_data2( 'icodtinicioitem[]', 'S', $habilita, '', 'S','',' verificaDataInicio(this); ','',' verificaDataInicio(this); ' ); ?></td>
                        <td align="center"><?php echo campo_data2( 'icodterminoitem[]', 'S', $habilita, '', 'S','',' verificaDataTermino(this); ','',' verificaDataTermino(this); ' ); ?></td>
                        <td align="center"><?php echo campo_texto( 'icovlritem[]', 'S', (obraMi($obrid)) ? 'N' : $habilita, '', 14, 14, '', '', 'right', '', 0, 'onkeypress="calculoEdificacao.setDefaultValue( this )";', 'calculoEdificacao.manager( this );'); ?></td>
                        <td align="center"><?php echo campo_texto( 'icopercsobreestrutura[]', 'N', 'N', '', 5, 6, '', '', 'right', '', 0); ?> %</td>
                    </tr>
                    <tr id="tr_total" bgcolor="#FFFFFF">
                        <td></td>
                        <td></td>
                        <td></td>
                        <td></td>
                        <td align="right"><strong>Total</strong></td>
                        <td align="center">
                            <input class='disabled' style="text-align:right" type='text' id='totalv' size='15' maxlength='14' value='<?php echo number_format($somav,2,',','.'); ?>' disabled="disabled">
                        </td>
                        <td align="center">
                            <input class='disabled' style="text-align:right" type='text' id='total' size='6' maxlength='6' value='<?php echo number_format($soma,2,',','.'); ?>' disabled="disabled"> %
                        </td>
                    </tr>
                    <tr id="tr_vlcontrato" bgcolor="#FFFFFF">
                        <td></td>
                        <td></td>
                        <td></td>
                        <td></td>
                        <td align="right"><strong>Valor do Contrato <?=(obraMi($obrid) ? '(sem os serviços externos)' : '')?></strong></td>
                        <td align="center">
                            <input class='disabled' style="text-align:right" type='text' name='vl_contrato' id='vl_contrato' size='15' maxlength='14' value='<?php echo number_format($ocrvalorexecucao,2,',','.'); ?>' disabled="disabled">
                        </td>
                        <td align="center">
                            <input class=disabled style="text-align:right" type='text' id='vl_porcento' size='6' maxlength='6' value='100,00' disabled="disabled"> %
                        </td>
                    </tr>
                    <tr id="tr_vlrestante" bgcolor="#FFFFFF">
                        <td colspan="4">
                            <?php if( $habilitado ): ?>
                                <a href="#" onclick="inserirEtapas();">
                                    <img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir Etapa">&nbsp;&nbsp;Inserir Etapa
                                </a>
                            <?php endif; ?>
                        </td>
                        <td align="right"><strong>Valor Restante</strong></td>
                        <td align="center">
                            <input class='disabled' style="text-align:right" type='text' id='rest_totalv' size='15' maxlength='14' value='<?php echo number_format($ocrvalorexecucao-$somav,2,',','.'); ?>' disabled="disabled">
                        </td>
                        <td align="center">
                            <input class='disabled' style="text-align:right" type='text' id='rest_total' size='6' maxlength='6' value='<?php echo number_format(100-$soma,2,',','.'); ?>' disabled="disabled"> %
                        </td>
                    </tr>
                </table>
            </td>

        </tr>
        <tr>
            <td class="SubTituloDireita" width="30%" nowrap="nowrap">TODOS OS SERVIÇOS QUE COMPOEM A PLANILHA PACTUADA COM O FNDE FORAM CONTRATADOS?</td>
            <td>
                <input type="radio" name="obrcronogramaservicocontratado" id="obrcronogramaservicocontratado_s" value="S"
                        <?=( !$habilitado ? 'disabled="disabled"' : '' ) ?> <?php echo $obrcronogramaservicocontratado == 'S' ? 'checked="checked"' : '' ?> onclick="managerServicoContratado('S');">
                <label for="obrcronogramaservicocontratado_s">Sim</label>
                <input type="radio" name="obrcronogramaservicocontratado" id="obrcronogramaservicocontratado_n" value="N"
                        <?=( !$habilitado ? 'disabled="disabled"' : '' ) ?> <?php echo $obrcronogramaservicocontratado == 'N' ? 'checked="checked"' : '' ?> onclick="managerServicoContratado('N');">
                <label for="obrcronogramaservicocontratado_n">Não</label>
            </td>
        </tr>
        <tr id="tr_servico_contratado_justificativa" style="display: <?php echo ($obrcronogramaservicocontratado == 'N' ? '' : 'none') ?>">
            <td class="SubTituloDireita" width="30%" nowrap="nowrap" valign="top">Justificativa:</td>
            <td>
            <?php
                echo campo_textarea( 'obrcronogramaservicocontratadojustificativa', 'S', 'S', '', '70', '8', '2000', '' , 0, '', '', false, '', '');
            ?>
            </td>
        </tr>

    </table>
    </form>

    <?php
    /**
     * Demanda #231610
     */
    if(obraMi($obrid)){
        $ordemServico = new OrdemServicoMI();
        $dados_os_espelho = $ordemServico->getArrayServicosExternosImplantacao($obrid);

        if(!empty($dados_os_espelho)){
            ?>
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                <tr>
                    <td>
                        <center><label class="TituloTela" style="color:#000000;">Serviços Externos</label></center>
                    </td>
                </tr>
                <tr>
                    <td>
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
                            <tbody>
                            <?php
                            $vl_tot = 0;
                            foreach ($dados_os_espelho as $servico) {
                                echo '<tr>';
                                echo '<td>'.$servico['item'].'</td>';
                                echo '<td>'.$servico['valor_unitario'].'</td>';
                                echo '<td>'.$servico['quantidade'].'</td>';
                                echo '<td>'.$servico['unidade_medida'].'</td>';
                                echo '<td>'.$servico['valor_total'].'</td>';
                                echo '</tr>';

                                $vl = str_replace('.', '', $servico['valor_total']);
                                $vl = str_replace(',', '.', $vl);
                                $vl = (float)$vl;
                                $vl_tot += $vl;
                            }
                            echo '
                                     <tr>
                                         <th><b>TOTAL</b></th>
                                         <td>&nbsp;</td>
                                         <td>&nbsp;</td>
                                         <td>&nbsp;</td>
                                         <td>'.number_format($vl_tot,2,',','.').'</td>
                                     </tr>
                                     ';
                            ?>
                            </tbody>
                        </table>
                    </td>
                </tr>
            </table>
        <?php }} ?>

    <script type="text/javascript">


    function managerServicoContratado(valor){
        if ( valor == 'N' ){
            $('#tr_servico_contratado_justificativa').show();
        }else{
            $('#tr_servico_contratado_justificativa').hide();
            $('#obrcronogramaservicocontratadojustificativa').val('');
        }
    }

    function verificaDataInicio(data){
        var obData = new Data();

        if( obData.comparaData(data.value,$('#crtdatainicioLabel').html(),'<') ){
            alert('A data de início não pode ser menor que a data de início do projeto.');
            data.value = '';
            data.focus();
            return;
        }

        if( obData.comparaData(data.value,$('#crtdatafimLabel').html(),'>') ){
            alert('A data de início não pode ser maior que a data de término do projeto.');
            data.value = '';
            data.focus();
            return;
        }

        // verificando se a data de inicio não é maior que a data de termino
        var nome = ($(data).attr('id')).split('_');
        if(nome[1]){
            if( obData.comparaData(data.value,$('#icodterminoitem_'+nome[1]).val(),'>') ){
                alert('A data de início não pode ser maior que a data de término.');
                data.value = '';
                data.focus();
                return;
            }
        }

    }

    function validaFormContrato(){
        var obCalc       = new Calculo();
        var result = true;
        var msg    = '';

        var arDtInicio = new Array();
        $('[name*=icodtinicioitem]').not(':last')
                           .each(function (i, obj){
                               arDtInicio.push( obj.value );
                           });
        if ( jQuery.inArray('', arDtInicio ) >= 0 ){
            msg += '* O campo Data de Início é de preenchimento obrigatório.\n';
            result = false;
        }

        var arDtTermino = new Array();
        $('[name*=icodterminoitem]').not(':last')
                           .each(function (i, obj){
                               arDtTermino.push( obj.value );
                           });
        if ( jQuery.inArray('', arDtTermino ) >= 0 ){
            msg += '* O campo Data de Término é de preenchimento obrigatório.\n';
            result = false;
        }

        $('[name*=icodtinicioitem]').not(':last').each(function (i, obj){
            var id = new String($(obj).attr('id')).split('_');
            id     = id[1];

            var dtInicio = new String($(obj).val()).split('/');
            dtInicio     = new Date(dtInicio[2], new Number(dtInicio[1])-1, dtInicio[0]);

            var dtTermino = new String($('#icodterminoitem_'+id).val()).split('/');
            dtTermino     = new Date(dtTermino[2], new Number(dtTermino[1])-1, dtTermino[0]);

            if(dtInicio > dtTermino ){
                msg += '* Nenhuma Data de Início pode ser maior que a Data de Término.\n';
                result = false;
                return false;
            }
        });

        if ( $('[name=obrcronogramaservicocontratado]:checked').val() == 'N' ){
            if ( $('#obrcronogramaservicocontratadojustificativa').val() == '' ){
                msg += 'O campo justificativa é obrigatório.\n';
            }
        }

        if ( msg != '' ){
            alert('Validação do formulário:\n\n' + msg);
            return false;
        }else{
            $('#evt').val('salvar');
            return true;
        }
    }

    <?php
        $itens    = new ItensComposicaoObras();
        $arItens = $itens->getItemComposicaoByObra( $obrid );

        if( is_array($arItens) && count($arItens) > 0){
            foreach ($arItens as $key  =>  $data){
                $data['icodtinicioitem'] = formata_data( $data['icodtinicioitem'] );
                $data['icodterminoitem'] = formata_data( $data['icodterminoitem'] );
                $data = array_map('utf8_encode', $data);
                echo "gerenteEdificacao.addLine(".simec_json_encode($data)."); ";
            }
        }

    ?>
    </script>
<?
}
?>

<?php

function salvaAditivo(){
    global $db;
    $contrato = new Contrato( $_SESSION['obras2']['crtid'] );
    extract( $contrato->getDados() );
    // Replica Contrato inserindo crtidpai
    $contrato 			= new Contrato( $crtid );
    $contrato->crtidpai = $contrato->crtid;
    $contrato->crtid	= null;
    $contrato->usucpf	= $_SESSION['usucpf'];
    $crtidNovo 			= $contrato->salvar();

    $obraContrato 	 = new ObrasContrato();
    $obraContrato2 	 = new ObrasContrato();
    $obras 		  	 = new Obras();
    $itemComposicao  = new ItensComposicaoObras();
    $detalheItem	 = new DetalheItem();
    $previsaoDetalhe = new PrevisaoDetalheItem();


    // Salva a nova planilha de custos
    $arqidcusto = null;
    if ( $_FILES['arqidcusto'] && $_FILES['arquivo']['error'] == 0 ){
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        $file  		  = new FilesSimec(null, null, 'obras2');
        $returnUpload = $file->setUpload(null, "arqidcusto", false, false);

        $arqidcusto = $file->getIdArquivo();
    }elseif ( $_FILES['arqidcusto']['error'] != 0 && $_FILES['arqidcusto']['error'] != 4 ){
        $returnUpload = false;
    }

    // Busca obras vinculadas ao contrato
    $arOcrid = $obraContrato->listaIdByContrato( $crtid );
    foreach ($arOcrid as $ocrid){
        // Carrega Obra Contrato
        $obraContrato2->carregarPorId( $ocrid );

        // Replica Obra
        $obras->carregarPorId( $obraContrato2->obrid );
        $obrid			 = $obras->obrid;
        $obras->obridpai = $obras->obrid;
        $obras->obrid 	 = null;
        $obridNovo 		 = $obras->salvar();
        $obras->clearDados();

        // Replica Obra Contrato
        $obraContrato2->carregarPorId( $ocrid );
        $obraContrato2->crtid 		= $crtidNovo;
        $obraContrato2->obrid 		= $obridNovo;
        $obraContrato2->ocrid 		= null;
        $obraContrato2->salvar();
        $obraContrato2->clearDados();

        $arCamposNulo = array();
        // Carrega e atualiza Obra Contrato ATUAL
        $obraContrato->carregarPorId( $ocrid );
        $_POST['obrid'] 					= ($_POST['obrid'] ? $_POST['obrid'] : array());
        $obraContrato->ocraditivado 		= ( in_array($obrid, $_POST['obrid']) ? 't' : 'f');
        $obraContrato->arqidcusto 		    = ($arqidcusto) ? $arqidcusto : $obraContrato->arqidcusto;

        if ( $_POST['ocrnovoprazoexecucao'][$obrid] ){
            $obraContrato->ocrprazoexecucao = $_POST['ocrnovoprazoexecucao'][$obrid];
        }else{
            $obraContrato->ocrprazoexecucao = NULL;
            $arCamposNulo[]  				= 'ocrprazoexecucao';
        }

        if ( $_POST['ocrnovodtterminoexecucao'][$obrid] ){
            $obraContrato->ocrdtterminoexecucao = formata_data_sql($_POST['ocrnovodtterminoexecucao'][$obrid]);
        }else{
            $obraContrato->ocrdtterminoexecucao = NULL;
            $arCamposNulo[]  					= 'ocrdtterminoexecucao';
        }

        if ( $_POST['ocrnovovalorexecucao'][$obrid] ){
            $obraContrato->ocrvalorexecucao = str_replace(array(".",","),array("","."),$_POST['ocrnovovalorexecucao'][$obrid]);
        }else{
            $obraContrato->ocrvalorexecucao = NULL;
            $arCamposNulo[]  			    = 'ocrvalorexecucao';
        }

        if ( $_POST['ocrnovocustounitario'][$obrid] ){
            $obraContrato->ocrcustounitario = str_replace(array(".",","),array("","."),$_POST['ocrnovocustounitario'][$obrid]);
        }else{
            $obraContrato->ocrcustounitario = NULL;
            $arCamposNulo[]  			    = 'ocrcustounitario';
        }

        $obraContrato->salvar(true, true, $arCamposNulo);
        $obraContrato->clearDados();

        // Busca ItensComposicaoObras
        $arIcoid = $itemComposicao->getIdByObra( $obrid );
        foreach ( $arIcoid as $icoid ){
            // Replica ItensComposicaoObras
            $itemComposicao->carregarPorId( $icoid );
            $itemComposicao->obrid = $obridNovo;
            $itemComposicao->icoid = null;
            $icoidNovo 			   = $itemComposicao->salvar();
            $itemComposicao->clearDados();

            // Busca detalheItem
            $arDitid = $detalheItem->getAllID( $icoid );
            foreach ( $arDitid as $ditid ){
                // Replica detalheItem
                $detalheItem->carregarPorId( $ditid );
                $detalheItem->icoid = $icoidNovo;
                $detalheItem->ditid = null;
                $ditidNovo 			= $detalheItem->salvar();
                $detalheItem->clearDados();

                // Busca PrevisaoDetalheItem
                $arPdiid = $previsaoDetalhe->getIdByDetalheItem( $ditid );
                foreach ( $arPdiid as $pdiid ){
                    $previsaoDetalhe->carregarPorId( $pdiid );
                    $previsaoDetalhe->ditid = $ditidNovo;
                    $previsaoDetalhe->pdiid = null;
                    $previsaoDetalhe->salvar();
                }
            }
        }

    }

    // Insere Arquivo
    if ( $_FILES['arquivo'] && $_FILES['arquivo']['error'] == 0 ){
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        $file  		  = new FilesSimec(null, null, 'obras2');
        $returnUpload = $file->setUpload(null, "arquivo", false, false);

        $arqid = $file->getIdArquivo();
    }elseif ( $_FILES['arquivo']['error'] != 0 && $_FILES['arquivo']['error'] != 4 ){
        $returnUpload = false;
    }

    if ( $returnUpload == false ){
        if ( $_FILES['arquivo']['error'] == 3 ){
            echo('<script type="text/javascript">
                    alert(\'Falha no upload do arquivo: ' . $_FILES['arquivo']['name'] . '\nAnexe-o novamente!\');
                 </script>');
        }else{
            $db->rollback();

            enviaEmailChiavicatti( $arquivo );

            die('<script type="text/javascript">
                    alert(\'Devido a falha no upload de arquivo os dados não foram salvos.\nFavor repetir a operação.\');
                    window.location.href = window.location.href;
                 </script>');
        }
    }

    // Atualiza CONTRATO
    $contrato->carregarPorId( $crtid );

    $contrato->arqid 		            = ($arqid ? $arqid : null);
    $contrato->crtprazovigencia 		= ($contrato->crtprazovigencia ? $contrato->crtprazovigencia : 0);
    $contrato->crtsupressao 			= ( $_POST['trasupressao'] == 'S'  ? 't' : 'f' );
    $contrato->crtjustificativa 		= $_POST['trajustificativa'];
    $contrato->crtdenominacao 			= $_POST['tradsc'];
    $contrato->ttaid 					= $_POST['ttaid'];
    $contrato->crtdtassinaturaaditivo 	= formata_data_sql( $_POST['tradtassinatura'] );
    $contrato->crtprazovigencia 		= (!empty( $_POST['traprazovigencia'] ) ? ($contrato->crtprazovigencia + $_POST['traprazovigencia']) : $contrato->crtprazovigencia);
    $contrato->crtdttermino 			= (!empty( $_POST['traterminovigencia'] ) ? formata_data_sql( $_POST['traterminovigencia'] ) : $contrato->crtdttermino);
    $contrato->crtvalorexecucao 		= (!empty( $_POST['travlrfinalobra'] ) ? MoedaToBd( $_POST['travlrfinalobra'] ) : $contrato->crtvalorexecucao);
    $contrato->salvar();

    $db->commit();
}

function salvaCronograma(){
    global $db;
   //verificar se as data de inicio precisa ser alterada
    if( !empty($_POST['dtInicioObra'])){
        $arObra['obrinicio'] =  formata_data_sql($_POST['dtInicioObra']);
    }

    //verificar se as data de termino precisa ser alterada
    if( !empty($_POST['dtTerminoObra'])){
        $arObra['obrtermino'] =  formata_data_sql($_POST['dtTerminoObra']);
    }

    $arCamposNulo = array();
    if ( $_POST['obrcronogramaservicocontratado'] ){
        $arObra['obrcronogramaservicocontratado'] = $_POST['obrcronogramaservicocontratado'];
    }else{
        $arCamposNulo[]	= 'obrcronogramaservicocontratado';
        $arObra['obrcronogramaservicocontratado'] = null;
    }

    if ( $_POST['obrcronogramaservicocontratadojustificativa'] ){
        $arObra['obrcronogramaservicocontratadojustificativa'] = $_POST['obrcronogramaservicocontratadojustificativa'];
    }else{
        $arCamposNulo[] = 'obrcronogramaservicocontratadojustificativa';
        $arObra['obrcronogramaservicocontratadojustificativa'] = null;
    }

    $obrid = $_SESSION['obras2']['obrid'];

    if(!empty($obrid)){
        $obra  = new Obras($obrid);
        $obra->popularDadosObjeto( $arObra )->salvar(true, true, $arCamposNulo);

        $itens    = new ItensComposicaoObras();
        $allItens = $itens->getAllID($obrid);

        $removeItens = array_diff($allItens, $_POST['itcid']);

        $itens->desabilitarItens( $removeItens, $obrid );

        for ($i=0; $i < count( $_POST['itcid'] ); $i++){
            if(!empty($_POST['itcid'][$i])){
                $arDados = array(
                                    'icoid'                 => ($_POST['icoid'][$i] ? $_POST['icoid'][$i] : null),
                                    'itcid'                 => ($_POST['itcid'][$i] ? $_POST['itcid'][$i] : null),
                                    'obrid'                 => $obrid,
                                    'icopercsobreestrutura' => ($_POST['icopercsobreestrutura'][$i] ? desformata_valor( $_POST['icopercsobreestrutura'][$i] ) : null),
                                    'icovlritem'            => ($_POST['icovlritem'][$i] ? desformata_valor( $_POST['icovlritem'][$i] ) : null),
                                    'icodtinicioitem'       => ($_POST['icodtinicioitem'][$i] ? formata_data_sql($_POST['icodtinicioitem'][$i]) : null),
                                    'icodterminoitem'       => ($_POST['icodterminoitem'][$i] ? formata_data_sql($_POST['icodterminoitem'][$i]) : null),
                                    'icostatus'             => 'A',
                                    'icodtinclusao'         => (empty($_POST['icoid'][$i]) ? 'NOW()' : null),
                                    'icoordem'              => ($i + 1)
                                  );

                $itens->popularDadosObjeto( $arDados )->salvar();
                $itens->clearDados();
            }
        }
        $itens->commit();
        return true;
    }else{
        return false;
    }
}
?>