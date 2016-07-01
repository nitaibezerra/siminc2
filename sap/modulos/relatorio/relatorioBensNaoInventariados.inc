<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<link type="text/css" rel="stylesheet" href="../fabrica/js/autocomplete/jquery.ui.base.css" />
<link type="text/css" rel="stylesheet" href="../fabrica/js/autocomplete/jquery.ui.theme.css" />
<script src="../fabrica/js/autocomplete/jquery-1.7.2.js"></script>
<script src="../fabrica/js/autocomplete/jquery.ui.core.js"></script>
<script src="../fabrica/js/autocomplete/jquery.ui.widget.js"></script>
<script src="../fabrica/js/autocomplete/jquery.ui.position.js"></script>
<script src="../fabrica/js/autocomplete/jquery.ui.autocomplete.js"></script>

<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>

<style>
        .ui-autocomplete-loading { background: white url('../fabrica/imagens/ui-anim_basic_16x16.gif') right center no-repeat; }
	.ui-autocomplete {
		max-height: 100px;
		overflow-y: auto;
		/* prevent horizontal scrollbar */
		overflow-x: hidden;
		/* add padding to account for vertical scrollbar */
		padding-right: 10px;
                background-color: #FFF;
                width: 300px;
                list-style:none;   
                
	}
	/* IE 6 doesn't support max-height
	 * we use height instead, but this forces the menu to always be this tall
	 */
	* html .ui-autocomplete {
		height: 100px;
	}
   
	</style>
<?php 
include APPRAIZ . 'includes/cabecalho.inc';
print "<br/>";
//Monta o Título da página abaixo da aba
monta_titulo("Relatório Levantamento de Bens Não Inventariados ", obrigatorio()." Indica campos obrigatórios.");
?>

<script type="text/javascript">
/**
 * Aciona o filtro
 * @name filtrar
 * @param requisicao - Requisição que será executada
 * @return void
 */
function filtrar(requisicao){

	$('requisicao').setValue(requisicao);
	$('formularioPesquisa').submit();

}
function visualizarRelatorio(){
    document.formularioPesquisa.action = 'sap.php?modulo=relatorio/bensNaoInventariadosPopup&acao=A';
    window.open( '', 'geraTermo', 'width=1600,height=1200,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
    document.formularioPesquisa.target = 'geraTermo';
    document.formularioPesquisa.submit();
    document.formularioPesquisa.focus();
}
</script>
<form name="formularioPesquisa" id="formularioPesquisa" b method="post">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
    <td colspan='2' bgcolor="#e9e9e9" align="center" style=""><b>Opções</b></td>
</tr>
  <tr>
    <td align='right' class="SubTituloDireita">Matricula SIAPE:</td>
    <td>
        <?php echo campo_texto('nu_matricula_siape','','','',40,40,'#######','','left','',0,'id="nu_matricula_siape"');?>         
        &nbsp;&nbsp;&nbsp;
        <input type="checkbox" name="nao_inventariados_todas" id="nao_inventariados_todas"> Todas
    </td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita ui-widget">Nome:</td>
    <td>
        <?php echo campo_texto('no_servidor','','','',40,40,'','','left','',0,'id="no_servidor"');?>
      <div id="return_servidor" style="color: red; display: none">Servidor não Encontrado!</div>
    </td>
  </tr>
    <tr>
        <td colspan='2' bgcolor="#e9e9e9" align="center" style=""><b>Localização</b></td>
    </tr>
<tr>
    <td align='right' class="SubTituloDireita">Unidade:</td>    
    <td>
        <?php 
        $sql = "SELECT uorco_interno_uorg as codigo, uorno as descricao FROM siorg.unidadeorganizacional order by uorno;";
        echo $db->monta_combo('unidade',$sql, 'S', 'Selecione', '', '', '', '', '','unidade', '', '','');
        ?>
    </td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Endereço:</td>    
    <td>
        <?php 
        $sql = "select 
                        siorg.endid as codigo,
                        endlog::varchar || ', ' || endcom::varchar as descricao 
                from 
                        siorg.endereco siorg
                order by 
                        descricao asc";
        echo $db->monta_combo('endereco',$sql, 'S', 'Selecione', '', '', '', '', '','endereco', '', '', '');
        ?>
    </td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Andar:</td>
    <td>
        <select disabled="disabled" name="endereco_andar" id="endereco_andar" class="CampoEstilo">
            <option value="">Selecione</option>
        </select>
        &nbsp;Sala:&nbsp;
        <select disabled="disabled" name="endereco_sala" id="endereco_sala" class="CampoEstilo">
            <option value="">Selecione</option>
        </select>
    </td>
  </tr>
<tr>
<td colspan='2' bgcolor="#e9e9e9" align="center" style=""><b>Bem</b></td>
</tr>
    <td ></td>
    <td>
        <input type='submit' class="botao" name='Visualizar' id="Visualizar" value='Visualizar' onclick="visualizarRelatorio();javascript: window.location.href = window.location" />
        <input type='reset' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar Campos' />
    </td>
  </tr>
</table>  
</form>

<script type="text/javascript">
$(document).ready(function(){
        $('#nu_matricula_siape').blur(function() {
        if ($("#nu_matricula_siape").val() != ""){   
            var nu_matricula_siape = $("#nu_matricula_siape").val();
            
            $.ajax({
                        type: 'post',
                        data: { tipo: "matriculaSiape", nu_matricula_siape: nu_matricula_siape },
                        //dataType: "json",
                        url:'../sap/geral/relatorios/relatorioBensInventariados.php',
                        success: function(result){
                            
                            if(result == ""){
                                $("#no_servidor").val('');
                                    $('#return_servidor').show('slow', function() {
                                    // Animation complete.
                                    });
                                
                            }else{
                                 $("#no_servidor").val(result);
                                 $('#return_servidor').hide('slow', function() {
                                    // Animation complete.
                                 });
                            }
                           
                            
                        }
                    })
                    
            }else{
                if ($("#nu_matricula_siape").val() == ""){
                    $("#no_servidor").val('');
                    $('#return_servidor').hide('slow', function() {
                                    // Animation complete.
                    });
                }
            }
        });
        
        $(function() {
            $( "#no_servidor" ).autocomplete({
                source: "../sap/geral/relatorios/relatorioBensInventariados.php?tipo=noServidor",
                minLength: 2,
                change: function(event, ui) { 
                    if(ui.item){

                    }else{
                        $("#nu_matricula_siape").val('');
                        $("#no_servidor").val('');
                    }
                },
                select: function( event, ui ) {
                        $("#nu_matricula_siape").val(ui.item.id)
                        $("#no_servidor").val(ui.item.value)
                            $('#return_servidor').hide('slow', function() {
                            // Animation complete.
                            });
                }
            });
	});
        
        $('#no_servidor').blur(function() {
            if($("#no_servidor").val() == ""){
                $("#nu_matricula_siape").val('');
                $("#no_servidor").val('');
                 $('#return_servidor').hide('slow', function() {
                                    // Animation complete.
                });
            }
        });
        
    $('#nao_inventariados_todas').click (function ()
    {
        if($('#nao_inventariados_todas').attr('checked')=="checked"){
            $("#nu_matricula_siape").attr('disabled','disabled').val('');
            $("#no_servidor").attr('disabled','disabled').val('');
             $('#return_servidor').hide('slow', function() {
                                    // Animation complete.
                });
        }else{
            $('#nu_matricula_siape').val('').removeAttr('disabled','disabled');
            $('#no_servidor').val('').removeAttr('disabled','disabled');
        }       
    });
        
    var endereco = "";
    $("#endereco").change(function () {
         endereco = $("#endereco").val();
        /*AJAX*/
        if (endereco != ""){
                $('#endereco_andar').removeAttr('disabled','disabled');
                $("#endereco_sala").empty().attr('disabled','disabled');
                $("<option value=\"\">Selecione</option>").appendTo("select#endereco_sala");
            
            $.ajax({
                type: 'post',
                data: { tipo: "endereco", endereco: endereco },
                dataType: "json",
                url:'../sap/geral/relatorios/relatorioBensInventariados.php',
                success: function(result){
                    
                    $('#endereco_andar option').each(function(i, option) {
                        $(option).remove();
                    });
                    
                    $('<option value="" selected="selected">Selecione</option>').appendTo("select#endereco_andar");
                    $.each(result, function(i, j) {
                        var row = "<option value=\"" + j.codigo + "\">" + j.descricao + "</option>";
                        $(row).appendTo("select#endereco_andar");
                    });
                    
                    $("#endereco_andar").removeAttr('disabled','disabled');
                }
            })
            
        }else{
        
            $("#endereco_andar").empty().attr('disabled','disabled');
            $("<option value=\"\">Selecione</option>").appendTo("select#endereco_andar");
            $("#endereco_sala").empty().attr('disabled','disabled');
            $("<option value=\"\">Selecione</option>").appendTo("select#endereco_sala");
        }
        
    });
    
    var endereco_andar = "";
    $("#endereco_andar").change(function () {
         endereco_andar = $("#endereco_andar").val();
        /*AJAX*/
        if (endereco_andar != ""){
            
            $('#endereco_sala').removeAttr('disabled','disabled');
            
            $.ajax({
                type: 'post',
                data: { tipo: "enderecoSala", endereco_andar: endereco_andar },
                dataType: "json",
                url:'../sap/geral/relatorios/relatorioBensInventariados.php',
                success: function(result){
                    console.log(result);
                    $('#endereco_sala option').each(function(i, option) {
                        $(option).remove();
                    });
                    
                    $('<option value="" selected="selected">Selecione</option>').appendTo("select#endereco_sala");
                    $.each(result, function(i, j) {
                        var row = "<option value=\"" + j.codigo + "\">" + j.descricao + "</option>";
                        $(row).appendTo("select#endereco_sala");
                    });
                    
                    $("#endereco_sala").removeAttr('disabled','disabled');
                }
            })
            
        }else{
        
            $("#endereco_sala").empty().attr('disabled','disabled');
            $("<option value=\"\">Selecione</option>").appendTo("select#endereco_sala");
        }
        
    });

})        
</script>