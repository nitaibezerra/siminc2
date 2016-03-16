<?php
$_SESSION['obras2']['obrid'] = $_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid'];
$empid = $_SESSION['obras2']['empid'];
$obrid = $_SESSION['obras2']['obrid'];

include  APPRAIZ."includes/cabecalho.inc";
$titulo_modulo    = 'Lista de Evoluções MI';
$subtitulo_modulo = 'Pesquisa dos registros de todas as Obras';
monta_titulo($titulo_modulo, $subtitulo_modulo);

//Fluxo de Evolução MI
$tpdid = TPDID_SUPERVISAO_MI;

?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">

function getEstados(){
    var estados = '';

    var elemento = document.getElementsByName('slEstado[]')[0];

    for (var i = 0; i < elemento.options.length; i++) {
        if (elemento.options[i].value != '')
        {
            estados += "'" + elemento.options[i].value + "',";
        }
    }

    return estados;
}

function ajaxEstado(){
    jQuery.ajax({
        type: "POST",
        url: window.location,
        data: "requisicaoAjax=filtrarMunicipio&estados=" + getEstados(),
        success: function(retorno) {
            jQuery('#idMunicipio').html(retorno);
        }});
}

function onOffCampo(campo){
    var div_on = document.getElementById(campo + '_campo_on');
    var div_off = document.getElementById(campo + '_campo_off');
    var input = document.getElementById(campo + '_campo_flag');
    if (div_on.style.display == 'none')
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

/**
 * Alterar visibilidade de um bloco.	 
 * @param string indica o bloco a ser mostrado/escondido
 * @return void
 */
function onOffBloco(bloco){
    var div_on = document.getElementById(bloco + '_div_filtros_on');
    var div_off = document.getElementById(bloco + '_div_filtros_off');
    var img = document.getElementById(bloco + '_img');
    var input = document.getElementById(bloco + '_flag');
    if (div_on.style.display == 'none')
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

function getLista(tipo){
    document.getElementById('tipo_relatorio').value = '';
    var formulario     = document.formulario;
    var tipo_relatorio = tipo;
    prepara_formulario();
    document.getElementById('tipo_relatorio').value = tipo_relatorio;
    formulario.submit();
}
</script>
    
<form name="formulario" id="formulario" action="" method="post">
    <input type="hidden" name="form" value="1" /> 
    <input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value="" /> 
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
        <tr>
            <td class="subtituloDireita"  style="width: 45%">Nome da Obra/ID:</td>
            <td>
                <?php 
                    $val = (!empty($_POST['obrbuscatexto'])) ? $_POST['obrbuscatexto'] : '';
                    echo campo_texto('obrbuscatexto', 'N', 'S', '', 70, 80, '', '', '', '', '', 'id="obrbuscatexto"', '', $val); 
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="35%">Empresa</td>
            <td>
                <?php
                $valEmi = (!empty($_POST['emiid'])) ? $_POST['emiid'] : '';
                $empresami = new EmpresaMi();
                $sql = $empresami->listaCombo(false);
                //                                $sql = $empresami->listaComboComEstuf();
                $db->monta_combo("emiid", $sql, 'S', "Selecione...", "", '', '', '', 'N', $valEmi);
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Situação:</td>
            <td>
                <?php
                    $sql = ' 
                            SELECT esdid as codigo, esddsc as descricao
                            FROM workflow.estadodocumento
                            WHERE tpdid = '.$tpdid.' 
                            ORDER BY esdid
                           ';
                    $val = (!empty($_POST['esdid'])) ? $_POST['esdid'] : '';
                    $db->monta_combo("esdid", $sql, "S", "Todas", "", "", "", 200, "N", "esdid", false, $val);
                ?>
            </td>
        </tr>
        <tr id="idUF">
                <?php 
                    #UF
                    $ufSql = " SELECT 	estuf as codigo, estdescricao as descricao
                               FROM territorios.estado est
                               ORDER BY estdescricao
                             ";
                    $stSqlCarregados = '';
                    $arr_uf = array();
                    if( $_POST['estuf'][0] != '' ){
                        foreach ($_POST['estuf'] as $key => $value) {
                            $arr_uf[$key] = "'".$value."'";
                        }
                        $stSqlCarregados = "SELECT
                                                    estuf as codigo, estdescricao as descricao
                                            FROM territorios.estado est
                                            WHERE 
                                                    estuf IN (".implode(',', $arr_uf).")
                                            ORDER BY
                                                    2";
                    }
                    mostrarComboPopup('UF:', 'estuf', $ufSql, $stSqlCarregados, 'Selecione o(s) Estado(s)', null, 'ajaxEstado', false);
                ?>
        </tr>
        <tr id="idMunicipio">
                <?php
                    #Municipio
                    $munSql = " SELECT muncod as codigo, mundescricao as descricao 
                                FROM territorios.municipio ";
                    $stSqlCarregados = '';
                    $arr_muncod = array();
                    if(is_array($_POST['muncod'])){
                        foreach ($_POST['muncod'] as $key => $value) {
                            $arr_muncod[$key] = "'".$value."'";
                        }
                    }
                    
                    $where_mun = (!empty($_POST['muncod'])) ? " muncod IN (".implode(',', $arr_muncod).") " : '';
                    $where_uf  = (!empty($arr_uf))          ? " estuf IN (".implode(',', $arr_uf).") "       : '';
                    
                    $where = '';
                    if(trim($where_mun) != ''){
                        $where .= $where_mun;
                    }
                    
                    if(trim($where_uf) != '' && trim($where) !== ''){
                        $where .= ' AND '.$where_uf;
                    }elseif(trim($where_uf) !== ''){
                        $where .= $where_uf;
                    }
                    
                    if(trim($where) !== ''){
                        $stSqlCarregados = "SELECT
                                                    muncod as codigo, mundescricao as descricao 
                                            FROM territorios.municipio
                                            WHERE 
                                                    {$where}
                                            ORDER BY
                                                    2";
                    }
                    mostrarComboPopup('Município:', 'muncod', $munSql, $stSqlCarregados, 'Selecione os municípios', null, '', false);
                ?>
        </tr>
        <tr>
            <td class="subtituloDireita"  style="width: 45%">Possui Foto ?</td>
            <td>
                <?php
                    switch ($_POST["evo_foto"]) {
                        case 'S':
                            $evo_foto_s = 'checked="checked"';
                            $evo_foto_n = '';
                            $evo_foto_t = '';
                            break;
                        case 'N':
                            $evo_foto_n = 'checked="checked"';
                            $evo_foto_s = '';
                            $evo_foto_t = '';
                            break;
                        case 'T':
                            $evo_foto_t = 'checked="checked"';
                            $evo_foto_s = '';
                            $evo_foto_n = '';
                            break;
                        default:
                            $evo_foto_t = 'checked="checked"';
                            $evo_foto_s = '';
                            $evo_foto_n = '';
                            break;
                    }
                ?>
                <input type="radio" name="evo_foto" id="" value="S" <?php echo $evo_foto_s;?> > Sim
                <input type="radio" name="evo_foto" id="" value="N" <?php echo $evo_foto_n;?> > Não
                <input type="radio" name="evo_foto" id="" value="T" <?php echo $evo_foto_t;?> > Todas
            </td>
        </tr>
        <tr>
            <td colspan="2" class="subtituloDireita">&nbsp;</td>
        </tr>
        <tr>
            <td colspan="2">
                <center>
                    <input type="button" id="btn_filtrar" name="btn_filtrar" value="Filtrar"      onclick="javascript:getLista('visual');"/>
                    <input type="button" id="btn_excel"   name="btn_excel"   value="Exportar XLS" onclick="javascript:getLista('xls');"/>
                </center>
            </td>
        </tr>

    </table>
    
</form>
    
<?php

if($_REQUEST['form'] == '1'){
    montaLista();
}

function montaLista(){
    global $db;
    $_SESSION['obras2']['obrid'] = 0;
    $_SESSION['obras2']['empid'] = 0;
    $evoMi     = new EvolucaoMi();
    $sql       = $evoMi->listaEvolucaoCompletaGeralSql($_POST);
    $cabecalho = array( //"ID Evolução", 
                        "ID Obra", 
                        "Estado",
                        "Município",
                        "Data da Evolução",
                        "Data de Inclusão",
                        "Responsável",
                        "% Medido (Edificação)",
                        "% Validado (Edificação)",
                        "Valor Medido (R$) (Edificação)",
                        "Valor Validado (R$) (Edificação)",
                        "% Medido (Serviços Externos)",
                        "% Validado (Serviços Externos)",
                        "Valor Medido (R$) (Serviços Externos)",            
                        "Valor Validado (R$) (Serviços Externos)",
                        "Possui Foto",
                        "Situação",
                        "Última tramitação");

    if($_REQUEST['tipo_relatorio'] == 'xls'){
        ini_set("memory_limit", "512M");
        $db->sql_to_xml_excel($sql, 'ListadeEvoluçõesMI', $cabecalho);
    }else{
        $db->monta_lista($sql, $cabecalho, 30, 10,'N','center', 'N', 'N');
    }    
}
?>