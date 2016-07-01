<?php
include APPRAIZ . 'includes/Agrupador.php';

$htmlObrigatorio = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';
monta_titulo('Lista de estudantes para pagamento' , 'Indica campo obrigatório '. $htmlObrigatorio);

$perfis = pegaPerfilGeral();
?>

<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-1.7.2.min.js"></script>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="./js/jquery-limit.js"></script>

<script type="text/javascript">
/* Função para substituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}
				
/**
 * Alterar visibilidade de um bloco.
 * 
 * @param string indica o bloco a ser mostrado/escondido
 * @return void
 */
function onOffBloco( bloco )
{
	var div_on = document.getElementById( bloco + '_div_filtros_on' );
	var div_off = document.getElementById( bloco + '_div_filtros_off' );
	var img = document.getElementById( bloco + '_img' );
	var input = document.getElementById( bloco + '_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '0';
		img.src = '/imagens/menos.gif';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '1';
		img.src = '/imagens/mais.gif';
	}
}

/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}
</script>
<link rel="stylesheet" type="text/css" href="./css/geral.css" />

<form name="frmEncaminharLista" id="frmEncaminharLista">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
            <td class="SubtituloDireita" width="25%">Período:</td>
            <td id="container-diario">
                <?php
                $sql = "SELECT DISTINCT 
							per.perid as codigo
			            	, per.perdescricao
			            	--|| ' - '
			            	--|| to_char(per.perdtinicio,'DD/MM/YYYY')
			            	--|| ' a '
			            	--|| to_char(per.perdtfim,'DD/MM/YYYY')
							as descricao
			        	FROM projovemcampo.diario dia
			        	INNER JOIN projovemcampo.periodo as per
			        	    ON dia.perid = per.perid
			        	ORDER BY per.perid";
				$db->monta_combo ( 'perid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'perid' );
                ?>
            </td>
		</tr><?php
		if( $db->testa_superuser()  || in_array(PFL_EQUIPE_MEC, $perfis) || in_array(PFL_CONSULTA, $perfis)||in_array(PFL_ADMINISTRADOR, $perfis) ){ ?>
		<tr>
            <td class="SubtituloDireita" width="25%">Estado do diário: </td>
            <td>
                <?php
                $arrayEstadoDocumento = array( ESTADO_DIARIO_ABERTO
											 , ESTADO_DIARIO_FECHADO
					                		 , ESTADO_DIARIO_COORDTURMA
					                		 , ESTADO_DIARIO_COORDGERAL
					                		 , ESTADO_DIARIO_DEVOLVIDO_DIRESCOLA
											 , ESTADO_DIARIO_DEVOLVIDO_COORDTURMA
											 , ESTADO_DIARIO_DEVOLVIDO_COORDGERAL);
                $estadosPendentes = implode(', ' , $arrayEstadoDocumento);
                $todos			= ESTADO_DIARIO_MEC.', '.ESTADO_DIARIO_PAGAMENTO_ENVIADO.', '.$estadosPendentes;

                $sqlEsdId = array( 	array('codigo'=> $todos, 'descricao'=>'Todos')
                					, array('codigo'=> ESTADO_DIARIO_MEC, 'descricao'=>'Recebido')
                					, array('codigo'=> $estadosPendentes, 'descricao'=>'Com pendência')
                					, array('codigo'=> ESTADO_DIARIO_ENVIADO_PAGAMENTO, 'descricao'=>'Encaminhados para pagamento') );
                $db->monta_combo('esdid', $sqlEsdId, 'S', 'Selecione', '', '', '', '', 'S', 'esdid');
                ?>
            </td>
		</tr>
		<?php
			$arrVisivel = array("descricao");
			$arrOrdem   = array("descricao");
			
			// Estado
			$stSql = " SELECT
							estuf AS codigo,
							estuf AS descricao
						FROM 
							territorios.estado";
			
			mostrarComboPopup( 'Estado:', 'estuf',  $stSql, '', 'Selecione o(s) Estado(s)' ,null,null,null,null,$arrVisivel,$arrOrdem,'S');
		?>
		<tr>
			<td class="SubTituloDireita">Município:</td>
			<td><? echo campo_texto('mundescricao', "N", "S", "", 37, 255, "", "", '', '', 0, 'id="mundescricao"' ); ?></td>
		</tr>
		<?php 
			// Escola
			$sql = "SELECT 
						ent.entid as codigo,
						'Escola '||ent.entid||',' ent.entnome as descricao 
					FROM 
						projovemcampo.turma tur
		  			INNER JOIN entidade.entidade ent ON tur.entid = ent.entid
		  			";
			mostrarComboPopup( 'Escola:', 'entid',  $sql, '', 'Selecione o(s) entleo(s)' ,null,null,null,null,$arrVisivel,$arrOrdem);				
		?>
		<tr>
			<td class="SubTituloDireita">Esfera:</td>
			<?
			$arExp = array(
							array("codigo" => 'T',
								  "descricao" => 'Todos'),
							array("codigo" => 'M',
								  "descricao" => 'Municipal'),
							array("codigo" => 'E',
								  "descricao" => 'Estadual')								
						  );
			?>
			<td><?=$db->monta_combo("esfera", $arExp, 'S','','', '', '','','N','esfera', '', '', 'Esfera'); ?></td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td>
				<input type="checkbox" name="estudantesaptos"   value="A" /> Estudantes Aptos
				<input type="checkbox" name="estudantesinaptos" value="I" /> Estudantes Inaptos
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td>
				<input type="checkbox" name="naopagamento" value="A" /> Não encaminhados para pagamento
				<input type="checkbox" name="simpagamento" value="I" /> Encaminhado para pagamento
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td>
				<input type="button" value="Visualizar" id="btVisualizar" />
			</td>
		</tr>
		
		<?php } ?><tr>
            <td colspan="2">
                <div id="container-encaminhar-lista"></div>
            </td>
        </tr>
    </table>
    
</form>


<div id="dialogAjax" >
    <img src="../imagens/wait.gif" alt="ajax"/>Aguarde! Carregando Dados...
</div>

<script type="text/javascript">
<?php
if( !$db->testa_superuser() && !in_array(PFL_EQUIPE_MEC, $perfis)  && !in_array(PFL_CONSULTA, $perfis) &&!in_array(PFL_ADMINISTRADOR, $perfis))
	{
	?>
	$(document).ready(function(){
		$('#perid').change(function(){

			var params     = {};
			
			params['acao']  = 'gerenciarListaDeEncaminhamento';
			params['perid'] = this.value;
			
			if( this.value != '')
            {
            	$.post( 'geral/ajax.php', params, function(response){
            		$('#container-encaminhar-lista').html( '' );
					$('#container-encaminhar-lista').html( response );
            	}, 'html' );
			}
		});

        jQuery.ajaxSetup({
            beforeSend: function(){
                $("#dialogAjax").show();
            },
            complete: function(){
                $("#dialogAjax").hide();
            }
        });
	});
	<?php
}else{ ?>
	$(document).ready(function(){

        jQuery.ajaxSetup({
            beforeSend: function(){
                $("#dialogAjax").show();
            },
            complete: function(){
                $("#dialogAjax").hide();
            }
        });

        $("#frmEncaminharLista").validate({

            //Define as regras dos campos
            rules:{
                    perid : {required: true},
                    esdid : {required: true}
            },
            //Define as mensagens de alerta
            messages:{
                    perid : 'Campo Obrigatório',
                    esdid : 'Campo Obrigatório'    
            }
        });

		$('#btVisualizar').click(function(){

			/*if( $('#estuf option:eq(0)').val() )
			{*/
		        if( $('#frmEncaminharLista').valid() == true )
		        {
					var params     = {};
	
					//Gatinho para selecionar o que já "está" selecionado
					
					$('#estuf option').attr('selected','selected');
					$('#entid option').attr('selected','selected');
	
					var nomeDoBotao	= $(this).val();
					$(this).val('Carregando...');
					$(this).attr('disabled','disabled');
	
					params['acao']  = 'gerenciarListaDeEncaminhamento';
					params['dados'] = $('#frmEncaminharLista').serialize();
	
	            	$.post( 'geral/ajax.php?acao=gerenciarListaDeEncaminhamento', $('#frmEncaminharLista').serialize(), function(response){
	                    $("#btEncaminharListaGeral").val( nomeDoBotao );
	                    $("#btEncaminharListaGeral").removeAttr('disabled');
	            		$('#container-encaminhar-lista').html( '' );
						$('#container-encaminhar-lista').html( response );
	            	}, 'html' );
		        }
			/*}else{
				alert('Campo Estado é obrigatório.')	
			}*/
		});		
	});
	<?php
}
?>	
</script>