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
monta_titulo("Relatório Bens Patrimoniais", obrigatorio()." Indica campos obrigatórios.");
?>

<script type="text/javascript">
/**
 * Aciona o filtro
 * @name filtrar
 * @param requisicao - Requisição que será executada
 * @return void
 */
$(document).ready(function(){

    function filtrar(requisicao){
            $('requisicao').setValue(requisicao);
            $('formularioPesquisa').submit();

    }

    $("#btnVisualizar").click( function(){
        $("#formularioPesquisa").attr('action', 'sap.php?modulo=relatorio/bensPatrimoniaisPopup&acao=A');
        $("#formularioPesquisa").attr('target', 'geraTermo' );
        window.open('', 'geraTermo', 'width=1600,height=1200,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
        $("#formularioPesquisa").submit();		
    });


});
</script>
<form name="formularioPesquisa" id="formularioPesquisa" b method="post">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
    <td colspan='2' bgcolor="#e9e9e9" align="center" style=""><b>Opções</b></td>
</tr>
  <tr>
    <td align='right' class="SubTituloDireita">Conta Contábil:</td>
    <td>
        <?php
            $sql = "SELECT ccbid as codigo, ccbdsc as descricao FROM sap.contacontabil order by ccbdsc asc";
            echo $db->monta_combo('contacontabil',$sql, 'S', 'Selecione', '', '', '', '', 'S','contacontabil', '', '','');
            ?>
        &nbsp;
        <input type="checkbox" name="contacontabil_todas" id="contacontabil_todas"> Todas
    </td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Item Conta Contábil:</td>
    <td>
        <select disabled="disabled" name="item_conta_contabil" id="item_conta_contabil" class="CampoEstilo">
            <option value="">Selecione</option>
        </select>
        &nbsp;
        <input type="checkbox" disabled="disabled" name="item_conta_contabil_todas" id="item_conta_contabil_todas"> Todas

    </td>
  </tr>
  <tr>
    <td colspan='2' bgcolor="#e9e9e9" align="center" style=""><b>Responsável</b></td>
</tr>
  <tr>
    <td align='right' class="SubTituloDireita">Matricula SIAPE:</td>
    <td>
       <?php echo campo_texto('nu_matricula_siape','','','',40,40,'#######','','left','',0,'id="nu_matricula_siape"');?>         
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
                        endereco.endid as codigo,
                        endlog::varchar || ', ' || endcom::varchar as descricao 
                from 
                        siorg.endereco endereco
                order by 
                        descricao asc";
        echo $db->monta_combo('endereco',$sql, 'S', 'Selecione', '', '', '', '', '','endereco', '', '','');
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
  <tr>
    <td align='right' class="SubTituloDireita">Descrição do Bem:</td>
    <td>
        <?php echo campo_texto('descricao_do_bem','','','',30,50,'','','left','',0,'id="descricao_do_bem"');?>
        <input type="hidden" name="descricao_do_bem_hidden" id="descricao_do_bem_hidden" value="" />
        <div id="return_descricao_do_bem" style="color: red;display: none">Não existe informações que atendam estes parâmetros.</div>
    </td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Estado de Conservação:</td>
    <td>
        
        
        <?php 
        $sql = "SELECT ecoid as codigo, ecodsc as descricao FROM sap.estadoconservacao order by ecodsc;";
        echo $db->monta_combo('estado_de_conservacao',$sql, 'S', '', '', '', '', '', '','estado_de_conservacao', '', '','');
        ?>
    </td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Situação do Bem:</td>
    <td>
        <?php 
        $sql = "
                SELECT 
                        TRIM(sbmcod) as codigo, 
                        sbmdsc as descricao 
                FROM 
                        SAP.SITUACAOBEM 
                ORDER 
                        BY SBMDSC;
                ";
        echo $db->monta_combo('situacao_do_bem',$sql, 'S', 'Todas', '', '', '', '', '','situacao_do_bem', '', '','');
        ?>
        &nbsp;
        <input type="checkbox" name="todas_situacao_bem" id="todas_situacao_bem" value=""> Todas
    </td>
  </tr>
    <tr>
        <td colspan='2' bgcolor="#e9e9e9" align="center" style=""><b>Classficar Relatório por</b></td>
    </tr>
  <tr>
    <td align='right' class="SubTituloDireita"></td>
    <td>
        <?
        $opcoes_tipo = array(
                'Nº RPG' => array(
                        'valor' => 'orderRgp',
                        'id' => 'orderRgp'
                ),
                'Nº do Termo' => array(
                        'valor' => 'orderTermo',
                        'id' => 'orderTermo'
                ),
                'Descrição do Bem' => array(
                        'valor' => 'orderDescricaodoBem',
                        'id' => 'orderDescricaodoBem'
                ),
                'Localização' => array(
                        'valor' => 'orderLocalizacao',
                        'id' => 'orderLocalizacao'
                )
        );
        ?>
        <? campo_radio( 'tipo_relatorio', $opcoes_tipo, 'h' ); ?>
        
    </td>
    </tr>
    <tr>
        <td colspan='2' bgcolor="#e9e9e9" align="center" style=""><b>Ocultar Colunas</b></td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita"></td>
    <td>
        <input type="checkbox" name="ocultarNtermo" value="ocultarNtermo" /> Nº do Termo<br />
        <input type="checkbox" name="ocultarResponsavel" value="ocultarResponsavel" /> Responsável<br />
        <input type="checkbox" name="ocultarSituacaoDoBem" value="ocultarSituacaoDoBem" /> Situação do Bem<br />
    </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita">
        <b>Bens com entrada Até:</b>
    </td>
        <td><?=campo_data('auddata_ini', '','S','','N');?></td>
  </tr>
  <tr bgcolor="#C0C0C0">
    <td ></td>
    <td>
        <input type='button' class="botao" name='btnVisualizar' id="btnVisualizar" value='Visualizar'/>
        <input type='reset' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar Campos' />
    </td>
    
  </tr>
</table>  
</form>

<script type="text/javascript">
$(document).ready(function(){
    var contacontabil = "";
    var coditem_conta_contabil = "";
    $("#contacontabil").change(function () {
        
        contacontabil = $("#contacontabil").val();
        if( $('#contacontabil_todas').attr('checked')=="checked" ) {
             $('#contacontabil_todas').attr('checked',false);
             $('#item_conta_contabil_todas').attr('checked',false);
             
        }
        /*AJAX*/
        if (contacontabil != ""){
            
            $('#item_conta_contabil_todas').removeAttr('disabled','disabled');
            $.ajax({
                type: 'post',
                data: { tipo: "contaContabil", contacontabil: contacontabil },
                dataType: "json",
                url:'../sap/geral/relatorios/relatorioBensInventariados.php',
                success: function(result){
                    
                    $('#item_conta_contabil option').each(function(i, option) {
                        $(option).remove();
                    });
                    
                    $('<option value="" selected="selected">Selecione</option>').appendTo("select#item_conta_contabil");
                    $.each(result, function(i, j) {
                        var row = "<option value=\"" + j.codigo + "\">" + j.descricao + "</option>";
                        $(row).appendTo("select#item_conta_contabil");
                    });
                    
                    $("#item_conta_contabil").removeAttr('disabled','disabled');
                }
            })
            
        }else{
        
            $("#item_conta_contabil").empty().attr('disabled','disabled');
            $("<option value=\"\">Selecione</option>").appendTo("select#item_conta_contabil");
        }
        
    });
    
    $("#item_conta_contabil").change(function () {
         coditem_conta_contabil = $("#item_conta_contabil").val();
            if( $('#item_conta_contabil_todas').attr('checked')=="checked" ) {
                    $('#item_conta_contabil_todas').attr('checked',false);
            }
    });
    
    $('#contacontabil_todas').click (function ()
    {
        if($('#contacontabil_todas').attr('checked')=="checked"){
            
            $('#contacontabil').val('');
            $("#item_conta_contabil").attr('disabled','disabled').val('');
            $('#item_conta_contabil_todas').attr('disabled','disabled').attr('checked',false);
        }else{
             if(contacontabil != ""){
                 $('#contacontabil').val(contacontabil);
                 $('#item_conta_contabil').val(coditem_conta_contabil);
                 $("#item_conta_contabil").removeAttr('disabled','disabled');
                 $('#item_conta_contabil_todas').removeAttr('disabled','disabled');
             }
        }       
    });
    
   $('#item_conta_contabil_todas').click (function ()
    {
        if($('#item_conta_contabil_todas').attr('checked')=="checked"){
            $('#item_conta_contabil').val('');
        }else{
            $('#item_conta_contabil').val(coditem_conta_contabil);
        }
    });
  
})
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

    //interaçao do combo "Situação do Bem:" e chekbox "Todas"
    var situacao_do_bem = "";
    $("#situacao_do_bem").change(function () {
    situacao_do_bem = $("#situacao_do_bem").val();
        if($("#situacao_do_bem").val() != "" && $('#todas_situacao_bem').attr('checked')=="checked"){
            $('#todas_situacao_bem').attr('checked',false);
        }
    }); 
    $('#todas_situacao_bem').click (function ()
    {
        if( $('#todas_situacao_bem').attr('checked')=="checked" ) {
            $('#situacao_do_bem').val('');
        }else{
             $('#situacao_do_bem').val(situacao_do_bem);
        }
    });
    
   $(function(){
		$( "#descricao_do_bem" ).autocomplete({
			source: "../sap/geral/relatorios/relatorioBensInventariados.php?tipo=descricaoBem",
			minLength: 2,
                        change: function(event, ui) { 
                            if(ui.item){
                                $('#return_descricao_do_bem').hide('slow', function() {
                                });
                            }else{
                                $('#return_descricao_do_bem').show('slow', function() {
                                     $('#descricao_do_bem_hidden').val('');
                                });
                            }
                        },
			select: function( event, ui ) {
                                $("#descricao_do_bem").val(ui.item.value);
                                $("#descricao_do_bem_hidden").val(ui.item.id);
			}
		});
                
	});
        
/*
$("#formularioPesquisa").validate({
	//Define as regras dos campos
	rules:{
			bendtentrada:{
				required: true
			},
			bendtsaida:{
				required: true
			},
			tipo:{
				required: true
			}
	},
	//Define as mensagens de alerta
	messages:{
		bendtentrada :"Campo Obrigatório",
		bendtsaida	 :"Campo Obrigatório",
		tipo		 :"Campo Obrigatório"
	}
});
*/
</script>