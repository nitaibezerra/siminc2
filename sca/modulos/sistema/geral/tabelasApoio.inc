<?php

ini_set("memory_limit", "1024M");
set_time_limit(0);

//Pega o sisid e cria a variável $sisid
$sisid = !$_SESSION['sisid'] ? $_REQUEST['sisid'] : $_SESSION['sisid'];

//Pega o schema e a table para edição
$shema = $_REQUEST['schema'];
$table = $_REQUEST['table'];

include APPRAIZ."sca/classes/ManterTabelaSCA.class.inc";

$tbl = new ManterTabelaSCA();

if($shema && $table){
    $_POST['combo_sch'] = $shema;
    $_POST['combo_tbl'] = $shema . '.' . $table;
}

if($_POST['combo_sch']){
    $tbl->setSchemaName($_POST['combo_sch']);
    if($_POST['combo_tbl']){
        $dado = explode(".",$_POST['combo_tbl']);
        $tbl->setTableName($dado[1]);
        if($_POST['inp_hdn_campo'] && $_POST['inp_hdn_campo_valor'] && $_POST['inp_hdn_acao'] == "carregar"){
            $arrDados = $tbl->popularDados();
        }

        if($_POST['inp_hdn_campo'] && $_POST['inp_hdn_campo_valor'] && $_POST['inp_hdn_acao'] == "excluir"){
            $tbl->excluirDados();
        }

        if($_POST['inp_hdn_acao'] == "salvar"){
            $tbl->salvarDados();
            unset($_POST['inp_hdn_acao']);
        }

        if($_POST['ajaxMethod']){
            header('content-type: text/html; charset=ISO-8859-1');
            $tbl->$_POST['ajaxMethod']();
            exit;
        }
    }
}

if(!$db->testa_superuser()){
    $tbl->setPermission(array("alterar"));
}

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';


$titulo = consultarTituloTela($abacod_tela, $url);
//echo "<pre>";print_r($titulo);exit;
monta_titulo( $titulo, '&nbsp' );

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript">

function esconde(id){
    $('#' + id ).fadeOut('slow');
}


$(function() {

     if($('#tr_msg_sucess')){
         window.setTimeout( "esconde('tr_msg_sucess')" , '5000' );
     }

     $('#btn_todos').click(function() {
         $("[name=txt_busca]").val("");
         $("#formulario_tabela_apoio").submit();
     });

     $('[class=link fechar]').click(function() {
         $('#' + this.parentNode.id ).fadeOut('slow');
     });

     $('[name=inp_table_description]').keypress(function(event){
         if(event.keyCode == 13){
                 if(!$('[name=inp_table_description]').val()){
                 alert('Informe a descrição da tabela.');
                 $('[name=inp_table_description]').focus();
             }else{
                 if( $('[name=inp_table_description]').val() == $('#span_table_description').html() ){
                     $('#div_table_description').fadeOut('slow');
                 }else{
                     $.ajax({
                       type: "POST",
                       url: window.location,
                       data: "combo_sch=" + $('[name=combo_sch]').val() + "&combo_tbl=" + $('[name=combo_tbl]').val() + "&ajaxMethod=saveTableDescription&params=" + $('[name=inp_table_description]').val(),
                       success: function(msg){
                               $('#span_table_description').html( $('[name=inp_table_description]').val() );
                            $('#div_table_description').fadeOut('slow');
                            return false;
                       }
                     });
                }
             }
             return false;
         }
     });

     $('#img_add_table_description').click(function() {
         $('[class~=absolute]').fadeOut('slow');

         if($('#img_add_table_description').attr("src") == "../imagens/icon_campus_2.png"){
             $('[name=inp_table_description]').val( $('#span_table_description').html() );
         }

         $('#div_table_description').fadeIn('slow');
     });

      $('#btn_table_description').click(function() {
         if(!$('[name=inp_table_description]').val()){
             alert('Informe a descrição da tabela.');
             $('[name=inp_table_description]').focus();
         }else{
             if( $('[name=inp_table_description]').val() == $('#span_table_description').html() ){
                 $('#div_table_description').fadeOut('slow');
             }else{
                 $.ajax({
                   type: "POST",
                   url: window.location,
                   data: "combo_sch=" + $('[name=combo_sch]').val() + "&combo_tbl=" + $('[name=combo_tbl]').val() + "&ajaxMethod=saveTableDescription&params=" + $('[name=inp_table_description]').val(),
                   success: function(msg){
                           $('#span_table_description').html( $('[name=inp_table_description]').val() );
                        $('#div_table_description').fadeOut('slow');
                        $('#img_add_table_description').attr("src","../imagens/icon_campus_2.png");
                   }
                 });
            }
         }
     });

     $('#btn_salvar').click(function() {
        var erro = 0;
        $("[class~=obrigatorio]").each(function() {
            if(!this.value){
                erro = 1;
                alert('Favor preencher o campo ' + this.title);
                this.focus();
                return false;
            }
        });
        if(erro == 0){
            $("#inp_hdn_acao").val("salvar");
            $("#formulario_tabela_apoio").submit();
        }
    });
});

function selecionaTabelaApoio(tabela){
    if(tabela){
        $("#formulario_tabela_apoio").attr("action","");
        $("#inp_hdn_salvar").val("0");
        $("#formulario_tabela_apoio").submit();
    }
}

function selecionaSchema(schema){
    if(schema){
        $("#formulario_tabela_apoio").attr("action","");
        $("#inp_hdn_salvar").val("0");
        $("#combo_tbl").val("");
        $("#formulario_tabela_apoio").submit();
    }
}

function editarTabela(campo,valor){
    if(campo && valor){
        $("#formulario_tabela_apoio").attr("action","");
        $("#inp_hdn_acao").val("carregar");
        $("#inp_hdn_campo").val(campo);
        $("#inp_hdn_campo_valor").val(valor);
        $("#inp_hdn_salvar").val("0");
        $("#formulario_tabela_apoio").submit();
    }
}

function excluirTabela(campo,valor){
    if(campo && valor && confirm("Deseja realmente excluir o registro?")){
        $("#formulario_tabela_apoio").attr("action","");
        $("#inp_hdn_acao").val("excluir");
        $("#inp_hdn_campo").val(campo);
        $("#inp_hdn_campo_valor").val(valor);
        $("#inp_hdn_salvar").val("0");
        $("#formulario_tabela_apoio").submit();
    }
}

function addTableColumnDesc(schema,table){
    if(schema && table){
        $('[class~=absolute]').fadeOut('slow');
        $('#div_desc_' + schema + "_" + table).fadeIn('slow');
    }else{
        alert('É necessário \'Schema\' e \'Tabela\' para essa operação!');
    }
}

function addListColumnTable(schema,table){
    if(schema && table){
        $('[class~=absolute]').fadeOut('slow');
        $('#div_list_' + schema + "_" + table).fadeIn('slow');
    }else{
        alert('É necessário \'Schema\' e \'Tabela\' para essa operação!');
    }
}

function salvarCampoDescTabela(column,schema,table,value){
    if(schema && table && $("#inp_desc_" + schema + "_" + table ).val()){
        $.ajax({
           type: "POST",
           url: window.location,
           data: "valueColumnTable=" + value + "&columnTable=" + column + "&combo_sch=" + $('[name=combo_sch]').val() + "&combo_tbl=" + $('[name=combo_tbl]').val() + "&ajaxMethod=saveColumnTableDescription&schema=" + schema + "&table=" + table + "&column=" + $("#inp_desc_" + schema + "_" + table ).val(),
           success: function(msg){
                   if(column){
                       $('#td_' + column ).html(msg);
                   }
           }
         });
    }else{
        alert('É necessário informar a coluna da tabela!');
    }
}

function salvarCampoListaTabela(schema,table){
    if(schema && table && $("#inp_list_" + schema + "_" + table ).val()){
        $.ajax({
           type: "POST",
           url: window.location,
           data: "txt_busca=" + $('[name=txt_busca]').val() + "&combo_sch=" + $('[name=combo_sch]').val() + "&combo_tbl=" + $('[name=combo_tbl]').val() + "&ajaxMethod=saveListColumnTable&column=" + $("#inp_list_" + schema + "_" + table ).val(),
           success: function(msg){
                   $("#div_list_" + schema + "_" + table ).fadeOut('slow');
                   $('#img_list_' + schema + '_' + table ).attr("src","../imagens/alterar.gif");
                   $("#divLista").html(msg);
           }
         });
    }else{
        alert('É necessário informar a(s) coluna(s) da tabela!');
    }
}

function editCommentColumn(column){

    $('[class~=absolute]').fadeOut('slow');
    $('#div_column_description_' + column).fadeIn('slow');
}

function saveColumnComment(column){
    $('[class~=absolute]').fadeOut('slow');
    if(column && $("[name=inp_column_description_" + column + "]").val()){
        $.ajax({
           type: "POST",
           url: window.location,
           data: "columnComment=" + $("[name=inp_column_description_" + column + "]").val() + "&column=" + column + "&combo_sch=" + $('[name=combo_sch]').val() + "&combo_tbl=" + $('[name=combo_tbl]').val() + "&ajaxMethod=saveColumnComment",
           success: function(msg){
                   $('#img_add_column_description_' + column ).attr("src","../imagens/icon_campus_2.png");

                   var valor = $('#inp_column_description_' + column).val();
                var posFim = valor.search("</label>");
                if(posFim > 0){
                    valor = valor.substr(0,posFim);
                    valor = valor.replace("<label>", "")
                    $('#name_column_' + column).html(valor);
                }

           }
         });
    }else{
        alert('É necessário informar a coluna da tabela!');
    }
}

function fechar(obj){
         $('#' + obj.parentNode.id ).fadeOut('slow');
}

function marcarTodas(obj,local){
    if(obj.checked == true){
        $("input[type=checkbox]","#" + local ).each(function(){
            if(this.id != obj.id)
            this.checked = true;
        });
    }else{
        $("input[type=checkbox]","#" + local ).each(function(){
            if(this.id != obj.id)
            this.checked = false;
        });
    }
}

</script>

<style>
    .SubtituloTabela{background-color:#cccccc}
    .negrito{font-weight:bold}
    .bold{font-weight:bold}
    .normal{font-weight:normal}
    .center{text-align: center;}
    .direita{text-align: right;}
    .esquerda{text-align: left;}
    .msg_erro{color:#990000}
    .link{cursor: pointer}
    .mini{width:12px;height:12px}
    .sucess_msg{color: blue;}
    .img_middle{vertical-align:middle}
    .hidden{display:none}
    .absolute{position:absolute;padding-top:5px;padding-bottom:5px;padding-left:5px;margin-top:5px;margin-left:50px;border:solid 1px black;background-color: #FFFFFF}
    .fechar{position:relative;right:-5px;top:-26px;}
    .img{background-color:#FFFFFF}
</style>

<form name="formulario_tabela_apoio" id="formulario_tabela_apoio"  method="post" action="" >
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <?php if($tbl->getMessage()): ?>
            <tr id="tr_msg_sucess" class="center SubtituloTabela" >
                <td colspan="2">
                    <?php echo $tbl->getMessage() ?>
                </td>
            </tr>
        <?php endif; ?>
        <?php if($_POST['combo_tbl']): ?>
            <?php $arrColunas = $tbl->getTableColumns();//dbg($tbl->getSQL())?>
            <?php if(is_array($arrColunas)):
                foreach($arrColunas as $coluna):?>
                <tr>
                    <td width="25%" class="SubtituloDireita" >
                        <?php if($coluna['comentario']):?>
                                <?php if(strstr($coluna['comentario'],"<label>") && strstr($coluna['comentario'],"</label>")):
                                    $coluna['label'] = null;
                                    $posLabel1 = strpos($coluna['comentario'],"<label>");
                                    $posLabel2 = strpos($coluna['comentario'],"</label>");
                                    $coluna['label'] = substr($coluna['comentario'],$posLabel1,$posLabel2);
                                    $coluna['label'] = str_replace(array("<label>","</label>"),"",$coluna['label']);
                                endif; ?>
                        <?php endif; ?>
                        <span id="name_column_<?php echo $coluna['coluna'] ?>"><?php echo $coluna['label'] ? $coluna['label'] : $coluna['coluna']?></span>
                        <?php if( in_array("comentar_coluna",$tbl->getPermission() ) ): ?>
                            <?php if($coluna['comentario']): ?>
                                <img title="Clique aqui para editar a descrição da coluna." src="../imagens/icon_campus_2.png" onclick="editCommentColumn('<?php echo $coluna['coluna'] ?>')" id="img_add_column_description_<?php echo $coluna['coluna'] ?>" class="link img_middle mini">
                            <?php else: ?>
                                <img title="Clique aqui para editar a descrição da coluna." src="../imagens/icon_campus_2_off.png" onclick="editCommentColumn('<?php echo $coluna['coluna'] ?>')" id="img_add_column_description_<?php echo $coluna['coluna'] ?>" class="link img_middle mini">
                            <?php endif; ?>
                            <div id="div_column_description_<?php echo $coluna['coluna'] ?>" class="hidden absolute" >
                                <?php if(1==2/*strlen($coluna['comentario']) < 80*/): ?>
                                    <?php $top = 12; ?>
                                    <?php echo campo_texto("inp_column_description_".$coluna['coluna'],"N","S","Descrição da Coluna","60","","","","","","","","",$coluna['comentario']); ?>
                                <?php else: ?>
                                    <?php $top = 92; ?>
                                    <?php echo campo_textarea("inp_column_description_".$coluna['coluna'],"N","S",'',"80","5",'','','','','','',$coluna['comentario']); ?>
                                <?php endif; ?>
                                <input type="button" id="btn_column_description_<?php echo $coluna['coluna'] ?>" onclick="saveColumnComment('<?php echo $coluna['coluna'] ?>')" name="btn_column_description_<?php echo $coluna['coluna'] ?>" value="OK" />
                                <img class="link fechar" style="top:-<?php echo $top ?>px" title="Fechar" src="../imagens/sair.gif" />
                            </div>
                        <?php endif; ?>
                    </td>
                    <td id="td_<?php echo $coluna['coluna'] ?>" ><?php echo $tbl->getInputColumn($coluna,( $arrDados[$coluna['coluna']] ? $arrDados[$coluna['coluna']] : null ) ); ?></td>
                </tr>
                <?php endforeach; ?>
                <input type="hidden" id="inp_hdn_salvar" name="inp_hdn_salvar" value="1" />
            <?php endif; ?>
                <tr>
                    <td class="SubtituloTabela" ></td>
                    <td class="SubtituloTabela" >
                        <input type="button" id="btn_salvar" value="Salvar" />
                    </td>
                </tr>
        <?php endif; ?>
    </table>
    <input type="hidden" id="inp_hdn_acao" name="inp_hdn_acao" value="" />
    <input type="hidden" id="inp_hdn_campo" name="inp_hdn_campo" value="" />
    <input type="hidden" id="inp_hdn_campo_valor" name="inp_hdn_campo_valor" value="" />
<?php if($arrColunas): ?>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td width="25%" class="SubTituloDireita" >Busca:</td>
            <td>
                <?php echo campo_texto('txt_busca','N','S','Busca',60,200,'','','','','','','',$_POST['txt_busca']); ?>
                <input type="submit" name="btn_buscar" id="btn_buscar" value="OK" />
                <?php if($_POST['txt_busca']): ?>
                    <input type="button" name="btn_todos" id="btn_todos" value="Mostrar Todos" />
                <?endif; ?>
            </td>
        </tr>
    </table>
</form>
<div id="divLista">
<?php $tbl->montaListaTabela($_POST['txt_busca']); ?>
</div>
<?php else: ?>
</form>
<?php endif;?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td>
            <?php $tbl->getError(); ?>
        </td>
    </tr>
</table>