<?php
if($_REQUEST['montafacilitador']){
	header('content-type: text/html; charset=ISO-8859-1');
	montaListaFacilitadorTurma( $_POST );
	exit;
}
if($_REQUEST['montaperiodo']){
	header('content-type: text/html; charset=ISO-8859-1');
	if( $_POST['turno'] == 'IN' ){
		echo campo_texto('turhrini', 'S', $habilita, 'Horário Inicial', 10, 5, '', '', '', '', 0, 'id=turhrini', "HoraMask('formulario', 'turhrini', event); pulaCampo('formulario', '5', 'turhrini', 'turhrfim', event)", '', "HoraValidationRoundMore('formulario','turhrini')" ).'&nbsp;- &nbsp;'.
					 campo_texto('turhrfim', 'S', $habilita, 'Horário Final', 10, 5, '', '', '', '', 0, 'id=turhrfim', "HoraMask('formulario', 'turhrfim', event); pulaCampo('formulario', '5', 'turhrfim', 'turhrinipm', event)", '', "HoraValidationRoundMore('formulario','turhrfim')" ).'&nbsp;&nbsp;&nbsp;Intervalo&nbsp;&nbsp;&nbsp;'.
					 campo_texto('turhrinipm', 'S', $habilita, 'Horário Inicial', 10, 5, '', '', '', '', 0, 'id=turhrinipm', "HoraMask('formulario', 'turhrinipm', event); pulaCampo('formulario', '5', 'turhrinipm', 'turhrfimpm', event)", '', "HoraValidationRoundMore('formulario','turhrinipm')" ).'&nbsp;- &nbsp;'.
					 campo_texto('turhrfimpm', 'S', $habilita, 'Horário Final', 10, 5, '', '', '', '', 0, 'id=turhrfimpm', "HoraMask('formulario', 'turhrfimpm', event); pulaCampo('formulario', '5', 'turhrfimpm', 'turcarga', event)", '', "HoraValidationRoundMore('formulario','turhrfimpm')" );
	} else {
		echo campo_texto('turhrini', 'S', $habilita, 'Horário Inicial', 10, 5, '', '', '', '', 0, 'id=turhrini', "HoraMask('formulario', 'turhrini', event); pulaCampo('formulario', '5', 'turhrini', 'turhrfim', event)", '', "HoraValidationRoundMore('formulario','turhrini')" ).'&nbsp;- &nbsp;'.
					 campo_texto('turhrfim', 'S', $habilita, 'Horário Final', 10, 5, '', '', '', '', 0, 'id=turhrfim', "HoraMask('formulario', 'turhrfim', event); pulaCampo('formulario', '5', 'turhrfim', 'turhrinipm', event)", '', "HoraValidationRoundMore('formulario','turhrfim')" );
	}
	exit;
}
if($_REQUEST['excluirfacilitador']){
	header('content-type: text/html; charset=ISO-8859-1');
	$sql = "DELETE FROM siscap.turma_facilitador WHERE facid = ".$_POST['facid']." and turid = ".$_POST['turid'];
	$db->executar( $sql );
	echo $db->commit();
	exit;
}
if($_REQUEST['salvarTurmaFacilitador']){
	header('content-type: text/html; charset=ISO-8859-1');
	insereTurma( $_POST, true );
	exit;
}

if( $_POST['requisicao'] == 'salvar' ){
	insereTurma( $_POST );
}

if( !empty($_REQUEST['turid']) ){
	$sql = "SELECT turid, curid, turdtinc, to_char(turdtini, 'DD/MM/YYYY') as turdtini, to_char(turdtfim, 'DD/MM/YYYY') as turdtfim, turturno, turhrini,
  					turhrfim, turperiodo, turpublicoalvo, turinfo, turstatus, turcarga, turmodalidade, turobs, turhrinipm, turhrfimpm
			FROM siscap.turma
			WHERE turid = ".$_REQUEST['turid'];
	$arTurma = $db->pegaLinha( $sql );

	extract( $arTurma );
}

$visualiza = $_REQUEST['visualizar'];

$habilita = $visualiza == 1 ? 'N' : 'S';
$habil = $habilita == 'N'  ? 'disabled="disabled"' : '';

if( $visualiza ){
	$titulo = 'Visualizar Turma';
} elseif( $_REQUEST['turid'] ){
	$titulo = 'Alterar Turma';
} else {
	$titulo = 'Cadastrar Turma';
}

include APPRAIZ . 'includes/cabecalho.inc';
print "<br>";

$habilitaCurso = 'S';
if( !empty($_GET['turid']) ) $habilitaCurso = 'N';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo, '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório.' );
?>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form method="post" id="formulario" name="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="habilita" id="habilita" value="<?=$habilita; ?>">
	<input type="hidden" name="turid" id="turid" value="<?=$_GET['turid']; ?>">
	<input type="hidden" name="datafimatual" id="datafimatual" value="<?=$turdtfim; ?>">
	<input type="hidden" name="dataatual" id="dataatual" value="<?=date( 'd/m/Y' ); ?>">

	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td width="15%" class="subtitulodireita">Curso:</td>
			<td colspan="3" width="35%">
				<?
				$sql = "SELECT curid as codigo, curdsc as descricao
						FROM siscap.curso
						ORDER BY curdsc";
				$db->monta_combo('curid', $sql, $habilitaCurso, 'Selecione...', '', '', '', '265', 'S', 'curid', '', '', 'Curso' );
				?>
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Turno:</td>
			<td colspan="3" width="35%">
				<?
				$arPeriodo = array(
								array("codigo" => 'MA', "descricao" => 'Matutino' ),
								array("codigo" => 'VE', "descricao" => 'Vespertino' ),
								array("codigo" => 'NO', "descricao" => 'Noturno' ),
								array("codigo" => 'IN', "descricao" => 'Integral' )
							);

				$db->monta_combo("turperiodo", $arPeriodo, $habilita, 'Selecione...', 'montaPeriodoPorTurno', '', '', '265', 'S', 'turperiodo', '', '', 'Turno' );
				?>
			</td>
		</tr>
        <tr>
            <td width="15%" class="subtitulodireita">Modalidade:</td>
            <td colspan="3" width="35%">
                <?
                $arModalidade = array(
                                array("codigo" => 'PR', "descricao" => 'Presencial' ),
                                array("codigo" => 'SP', "descricao" => 'Semipresencial' ),
                                array("codigo" => 'AD', "descricao" => 'A distância' )
                            );

                $db->monta_combo("turmodalidade", $arModalidade, $habilita, 'Selecione...', '', '', '', '265', 'S', 'turmodalidade', '', '', 'Turno' );
                ?>
            </td>
        </tr>
		<tr>
			<td width="15%" class="subtitulodireita">Data Inicial / Final:</td>
			<td colspan="3" width="35%">
				<?
				echo campo_data2('turdtini', 'S',$habilita,'Data Inicial','','','').'&nbsp;&nbsp;&nbsp;- &nbsp;&nbsp;&nbsp;'.
					 campo_data2('turdtfim', 'S',$habilita,'Data Final','','','');
				?>
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Horário Inicial / Final:</td>
			<td colspan="3" width="35%">
				<div id="turno"></div>
				<input type="hidden" name="turhrini1" id="turhrini1" value="<?=$turhrini; ?>">
				<input type="hidden" name="turhrfim1" id="turhrfim1" value="<?=$turhrfim; ?>">
				<input type="hidden" name="turhrinipm1" id="turhrinipm1" value="<?=$turhrinipm; ?>">
				<input type="hidden" name="turhrfimpm1" id="turhrfimpm1" value="<?=$turhrfimpm; ?>">
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Carga Horária:</td>
			<td colspan="3" width="35%">
				<?
				echo campo_texto('turcarga', 'S', $habilita, 'Carga Horária', 10, 3, '[#]', '', '', '', 0, 'id=turcarga' ).'&nbsp; Horas';
				?>
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Público Alvo:</td>
			<td colspan="3" width="35%">
				<?
				echo campo_textarea('turpublicoalvo', 'S', $habilita, 'Público Alvo', 80, 5, 500, '', '', '', '', 'Público Alvo');
				?>
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Status:</td>
			<td colspan="3" width="35%">
				<?
				$arStatus = array(
								array("codigo" => 'EA', "descricao" => 'Em andamento' ),
								array("codigo" => 'IA', "descricao" => 'Inscrições Abertas' ),
								array("codigo" => 'IE', "descricao" => 'Inscrições Encerradas' ),
								array("codigo" => 'CO', "descricao" => 'Concluído' ),
								array("codigo" => 'AD', "descricao" => 'Adiado' ),
								array("codigo" => 'CA', "descricao" => 'Cancelado' ),
							);
				$db->monta_combo("turstatus", $arStatus, $habilita, 'Selecione...', '', '', '', '265', 'S', 'turstatus', '', '', 'Status' );
				?>
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Outras Informações:</td>
			<td colspan="3" width="35%">
				<?
				echo campo_textarea('turinfo', 'N', $habilita, '', 80, 5, 500, '', '', '', '', 'Outras Informações');
				?>
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Observações:</td>
			<td colspan="3" width="35%">
				<?
				echo campo_textarea('turobs', 'N', $habilita, '', 80, 5, 500, '', '', '', '', 'Observações');
				?>
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Facilitador:</td>
			<td colspan="3" width="35%">
                <?php
                $turid = $_REQUEST['turid'];
                if($turid){

                    $sql = "SELECT
                                f.facid as codigo,
                                f.facnome as descricao
                            FROM siscap.facilitador f
                            INNER JOIN siscap.turma_facilitador t ON f.facid = t.facid
                            WHERE t.turid = {$turid}";

                    $facid = $db->carregar($sql);
                }

                $sqlFacilitador = "SELECT
                                        facid as codigo,
                                        facnome as descricao
                                   FROM siscap.facilitador";

                combo_popup( "facid", $sqlFacilitador, "Facilitador", "215x400", 0, "", "", "S", false, false, 5, 480 ,'','','','',$facid);

                ?>
                <!--
                <div id="facilitador"></div>
                -->
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
            <td></td>
            <td style="text-align: left">
				<?php if( $habilita == 'S' ){ ?>
					<input type="button" id="btSalvar" value="Salvar" onclick="salvarTurma();" />
				<?php } ?>
				<input type="button" id="btVoltar" value="Voltar" onclick="voltar();" />
			</td>
		</tr>
	</table>
</form>
<div id="erro"></div>
<script type="text/javascript">
montaListaFacilitador();
montaPeriodoPorTurno( '<?=$turperiodo; ?>' );

function salvarTurma(tipo){
	var nomeform 		= 'formulario';
	var submeterForm 	= false;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();

	campos[0] 			= "curid";
	campos[1] 			= "turperiodo";
	campos[2] 			= "turdtini";
	campos[3] 			= "turdtfim";
	campos[4] 			= "turhrini";
	campos[5] 			= "turhrfim";
	if( $('turperiodo').getValue() == 'IN' ){
		campos[6] 			= "turhrinipm";
		campos[7] 			= "turhrfimpm";
		tiposDeCampos[6] 	= "texto";
		tiposDeCampos[7] 	= "texto";
	}
	campos[8] 			= "turcarga";
	campos[9] 			= "turpublicoalvo";
	campos[10] 			= "turstatus";

	tiposDeCampos[0] 	= "select";
	tiposDeCampos[1] 	= "select";
	tiposDeCampos[2] 	= "texto";
	tiposDeCampos[3] 	= "texto";
	tiposDeCampos[4] 	= "texto";
	tiposDeCampos[5] 	= "texto";
	tiposDeCampos[8] 	= "texto";
	tiposDeCampos[9] 	= "textarea";
	tiposDeCampos[10] 	= "select";

	if(validaForm(nomeform, campos, tiposDeCampos, submeterForm ) ){
		if(!validaData($('turdtini') ) ) {
			alert('Data início está no formato incorreto.');
			$('turdtini').focus();
			return false;
		}else if(!validaData($('turdtfim') ) ) {
			alert('Data fim está no formato incorreto.');
			$('turdtfim').focus();
			return false;
		}else if( !validaDataMaior( $('turdtini'), $('turdtfim') ) ){
			alert("A data inicial não pode ser maior que data final.");
				$('turdtini').focus();
			return false;
		}

		if( $('turstatus').value == 'EA' || $('turstatus').value == 'IA' ){
			var data = new Date();
            var ano = data.getFullYear();

            var dataini = $('turdtini').value.split('/');
            var datafim = $('turdtfim').value.split('/');

			if( dataini[2] < ano || datafim[2] < ano ){
				alert('A Data Início/Data Final não pode ser menor que a Data Atual, \ncaso o usuário precise cadastrar histórico dos Cursos/Turmas, \nentão o Status Não pode ser "Em andamento"!');
				return false;
			}
		}
		var obDt  = new Data();
		var turdtfim = $('turdtfim').value;
		var turdtini = $('turdtini').value;

		if( $('turid').value != '' ){
			var datafimatual = $('datafimatual').value;
			var dataatual = $('dataatual').value;

			if( $('turstatus').value == 'CO' && obDt.comparaData( turdtfim, datafimatual, '>' ) ){
				alert('A Data Final não pode ser maior que a data atual quando a turma está	 concluído!');
				$('turdtfim').focus();
				return false;
			}
		}

		var turhrini = $('turhrini').value;
		var turhrfim = $('turhrfim').value;

		if(CompararHoras( $('turhrini').value, $('turhrfim').value ) == 'igual'){
			alert('Horário Inicial / Final não podem ser iguais!');
			$('turhrini').focus();
			return false;
		}
		if( obDt.comparaData( turdtini, turdtfim, '=' ) ){
			if(CompararHoras( $('turhrini').value, $('turhrfim').value ) == 'maior'){
				alert('Horário Inicial não pode ser maior que o Horário Final!');
				$('turhrini').focus();
				return false;
			}
		}

	    selectAllOptions( $('facid') );
		$('requisicao').value = 'salvar';
//		if( tipo == 'ajax' ){
//			var myajax = new Ajax.Request('siscap.php?modulo=principal/gerenciaTurma&acao=A', {
//				        method:     'post',
//				        parameters: '&salvarTurmaFacilitador=true&'+$('formulario').serialize(),
//				        asynchronous: false,
//				        onComplete: function (res){
//				        	//$('erro').innerHTML = res.responseText;
//				        	if( Number(res.responseText) ){
//				        		alert('Operação realizada com sucesso!');
//				        		$('turid').value = res.responseText;
//				        		$('curid').disabled = true;
//				        		montaListaFacilitador();
//				        	} else {
//				        		alert('Falha na Operação');
//				        	}
//				        }
//				  });
//		} else {
			$('formulario').submit();
//		}
	}
}


function CompararHoras(sHora1, sHora2) {

    var arHora1 = sHora1.split(":");
    var arHora2 = sHora2.split(":");

    // Obtener horas y minutos (hora 1)
    var hh1 = parseInt(arHora1[0],10);
    var mm1 = parseInt(arHora1[1],10);

    // Obtener horas y minutos (hora 2)
    var hh2 = parseInt(arHora2[0],10);
    var mm2 = parseInt(arHora2[1],10);

    // Comparar
    if (hh1<hh2 || (hh1==hh2 && mm1<mm2))
        return "menor";
    else if (hh1>hh2 || (hh1==hh2 && mm1>mm2))
        return "maior";
    else
        return "igual";
}

function onOffCampo( campo ){
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );

	if ( div_on.style.display == 'none' ){
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}else{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}
function voltar(){
	window.location.href = 'siscap.php?modulo=principal/listaTurma&acao=A';
}

function montaListaFacilitador(){
	var myajax = new Ajax.Request('siscap.php?modulo=principal/gerenciaTurma&acao=A', {
			        method:     'post',
			        parameters: '&montafacilitador=true&'+$('formulario').serialize(),
			        asynchronous: false,
			        onComplete: function (res){
						$('facilitador').innerHTML = res.responseText;
			        }
			  });
}

function montaPeriodoPorTurno( turno ){
	var myajax = new Ajax.Request('siscap.php?modulo=principal/gerenciaTurma&acao=A', {
			        method:     'post',
			        parameters: '&montaperiodo=true&turno='+turno,
			        asynchronous: false,
			        onComplete: function (res){
						$('turno').innerHTML = res.responseText;
						$('turhrini').setValue( $('turhrini1').getValue() );
						$('turhrfim').setValue( $('turhrfim1').getValue() );
						$('turhrinipm').setValue( $('turhrinipm1').getValue() );
						$('turhrfimpm').setValue( $('turhrfimpm1').getValue() );
			        }
			  });
}

function excluirFacilitadorTurma( facid, turid ){
	if(confirm("Deseja realmente excluir o facilitador?")) {
		var myajax = new Ajax.Request('siscap.php?modulo=principal/gerenciaTurma&acao=A', {
				        method:     'post',
				        parameters: '&excluirfacilitador=true&facid='+facid+'&turid='+turid,
				        asynchronous: false,
				        onComplete: function (res){
				        	//$('erro').innerHTML = res.responseText;
				        	if( res.responseText == '1' ){
				        		alert('Operação realizada com sucesso!');
				        		montaListaFacilitador();
				        	} else {
				        		alert('Falha na Operação');;
				        	}
				        }
				  });
	}
}
</script>
