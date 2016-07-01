<?php

switch ($_REQUEST['ajax']){
	case 'municipio':
		header('content-type: text/html; charset=ISO-8859-1');

		$municipio = new Municipio();
		echo $db->monta_combo("muncod", $municipio->listaCombo( array('estuf' => $_POST['estuf']) ), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
		exit;		
}

?>
        
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
        
<?php

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Relatório - Restrições/Inconformidades', 'Relatório - Restrições/Inconformidades' );

?>
    
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>    
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
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
    
    jQuery(function($){
        $.datepicker.regional['pt-BR'] = {
                closeText: 'Fechar',
                prevText: '&#x3c;Anterior',
                nextText: 'Pr&oacute;ximo&#x3e;',
                currentText: 'Hoje',
                monthNames: ['Janeiro','Fevereiro','Mar&ccedil;o','Abril','Maio','Junho','Julho','Agosto','Setembro','Outubro','Novembro','Dezembro'],
                monthNamesShort: ['Jan','Fev','Mar','Abr','Mai','Jun','Jul','Ago','Set','Out','Nov','Dez'],
                dayNames: ['Domingo','Segunda-feira','Ter&ccedil;a-feira','Quarta-feira','Quinta-feira','Sexta-feira','S&aacute;bado'],
                dayNamesShort: ['Dom','Seg','Ter','Qua','Qui','Sex','Sab'],
                dayNamesMin: ['Dom','Seg','Ter','Qua','Qui','Sex','Sab'],
                weekHeader: 'Sm',
                dateFormat: 'dd/mm/yy',
                firstDay: 0,
                isRTL: false,
                showMonthAfterYear: false,
                yearSuffix: ''};
        $.datepicker.setDefaults($.datepicker.regional['pt-BR']);
    });


</script>

<form action="obras2.php?modulo=relatorio/popupRelatorioRestricoesInconformidades&acao=A" method="post" name="filtro" id="filtro">
<input type="hidden" name="pesquisa" id="pesquisa" value="1"/>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">

    <tr>
        <td class="SubTituloDireita" style="width: 490px;">Nome da Obra/ID:</td>
        <td>
            <?php echo campo_texto( 'obrid', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
        </td>
    </tr>
    <tr>
        <td class="subtituloDireita" style="width: 40%;">Tipologia:</td>
        <td>
            <?php
                $tipologiaObra = new TipologiaObra();
                $param = array("orgid" => $_SESSION['obras2']['orgid']);
                $val = (!empty($_POST['tpoid'])) ? $_POST['tpoid'] : '';
                $db->monta_combo("tpoid", $tipologiaObra->listaCombo($param), "S", "Todas", "", "", "", 200, "N", "tpoid", false, $val);
            ?>
        </td>
    </tr>
    <tr>
        <td class="subtituloDireita">Item:</td>
        <td>
            <?php
                $select_r = '';
                $select_i = '';
                $select_t = '';
                switch ($_POST['item_restrict']) {
                    case 'R':
                        $select_r = 'checked="checked"';
                        break;
                    case 'I':
                        $select_i = 'checked="checked"';
                        break;
                    case 'T':
                        $select_t = 'checked="checked"';
                        break;
                }
            ?>
            <input type="radio" name="item_restrict" id="item_restrict_r" value="R" <?php echo $select_r; ?> > Restrição
            <input type="radio" name="item_restrict" id="item_restrict_i" value="I" <?php echo $select_i; ?> > Inconformidade
            <input type="radio" name="item_restrict" id="item_restrict_t" value="T" <?php echo $select_t; ?> > Todas
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Tipo de Restrição:</td>
        <td>
            <?php
            $sql = " SELECT tprid as codigo, tprdsc as descricao 
                     FROM obras2.tiporestricao
                     WHERE tprstatus = 'A' 
                     ORDER BY tprid";
            $db->monta_combo("tprid", $sql, "S", "Todos", "", "", "", "200", "N", "tprid");
            ?>
        </td>
    </tr>
    <!--            <td class="subtituloDireita">Situação da Restrição/Inconformidades:</td>
            <td>-->
           <?php
                    $sql = ' 
                            SELECT esdid as codigo, esddsc as descricao
                            FROM workflow.estadodocumento
                            WHERE tpdid = '.TPDID_RESTRICAO_INCONFORMIDADE;
                    $stSqlCarregados = '';
                    $arr_ri_wf = array();
                    if( !empty($_POST['esdid_ri']) && is_array($_POST['esdid_ri']) && $_POST['esdid_ri'][0] != ''){
                        foreach ($_POST['esdid_ri'] as $key => $value) {
                            $arr_ri_wf[$key] = "'".$value."'";
                        }
                        $str_colecao = (!empty($arr_ri_wf)) ? " AND esdid IN (".implode(',', $arr_ri_wf).") " : ''; 
                        $stSqlCarregados = "SELECT esdid as codigo, esddsc as descricao
                                            FROM workflow.estadodocumento
                                            WHERE tpdid = ".TPDID_RESTRICAO_INCONFORMIDADE." 
                                            {$str_colecao}
                                            ORDER BY
                                                    esdid";
                    }
                    mostrarComboPopup('Situação da Restrição/Inconformidades:', 'esdid_ri', $sql, $stSqlCarregados, 'Selecione a(s) Situação(ões)', null, '', false);
                    //$db->monta_combo("esdid", $sql, "S", "Todas", "", "", "", 200, "N", "esdid", false, $val);
                ?>
            <!--</td>-->
        </tr>
        <tr>
<!--            <td class="SubTituloDireita">Situação da Obra:</td>
            <td>-->
                <?php
                $sql_obr = "SELECT esdid as codigo, esddsc as descricao 
                            FROM workflow.estadodocumento 
                            WHERE tpdid='" . TPDID_OBJETO . "' 
                              AND esdstatus='A' 
                            ORDER BY esdordem";
                $stSqlCarregados = '';
                    $arr_obr_wf = array();
                    if( !empty($_POST['esdid_obr']) && is_array($_POST['esdid_obr']) && $_POST['esdid_obr'][0] != ''){
                        foreach ($_POST['esdid_obr'] as $key => $value) {
                            $arr_obr_wf[$key] = "'".$value."'";
                        }
                        $str_colecao = (!empty($arr_obr_wf)) ? " AND esdid IN (".implode(',', $arr_obr_wf).") " : ''; 
                        $stSqlCarregados = "SELECT esdid as codigo, esddsc as descricao 
                                            FROM workflow.estadodocumento 
                                            WHERE tpdid='" . TPDID_OBJETO . "' 
                                              AND esdstatus='A' 
                                            {$str_colecao}
                                            ORDER BY
                                                    esdid";
                    }
                    mostrarComboPopup('Situação da Obra:', 'esdid_obr', $sql_obr, $stSqlCarregados, 'Selecione a(s) Situação(ões)', null, '', false);
//                $db->monta_combo("esdid", $sql, "S", "Todos", "", "", "", "200", "N", "esdid");
                ?>
            <!--</td>-->
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
                    $munSql = " SELECT muncod as codigo, estuf || ' - ' || mundescricao as descricao 
                                FROM territorios.municipio 
                                ORDER BY
                                    mundescricao";
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
                                                    muncod as codigo, estuf || ' - ' || mundescricao as descricao 
                                            FROM territorios.municipio
                                            WHERE 
                                                    {$where}
                                            ORDER BY
                                                    mundescricao";
                    }
                    mostrarComboPopup('Município:', 'muncod', $munSql, $stSqlCarregados, 'Selecione os municípios', null, '', false);
                ?>
        </tr>
        <?php
        // Programa
        $stSql = "SELECT
                    prfid AS codigo,
                    prfdesc AS descricao
                 FROM 
                    obras2.programafonte
                 ORDER BY
                    prfdesc ";
        if(!empty($_POST['prfid'][0])){
            $stSqlSelecionados = "SELECT
                        prfid AS codigo,
                        prfdesc AS descricao
                     FROM 
                        obras2.programafonte
                     WHERE prfid IN (". implode(', ', $_POST['prfid']) .")
                     ORDER BY
                        prfdesc ";
        }
        mostrarComboPopup( 'Programa', 'prfid',  $stSql, $stSqlSelecionados, 'Selecione o(s) Programa(s)' );
        // Fonte
        $stSql = "SELECT
                    tooid AS codigo,
                    toodescricao AS descricao
                 FROM 
                    obras2.tipoorigemobra
                 WHERE
                    toostatus = 'A'
                 ORDER BY
                    toodescricao ";
        
        if(!empty($_POST['tooid'][0])){
            $sql_carregados = "SELECT
                        tooid AS codigo,
                        toodescricao AS descricao
                     FROM 
                        obras2.tipoorigemobra
                     WHERE
                        toostatus = 'A'
                        AND tooid IN (".  implode(', ', $_POST['tooid']).")
                     ORDER BY
                        toodescricao ";
        }
        mostrarComboPopup( 'Fonte', 'tooid',  $stSql, $sql_carregados, 'Selecione a(s) Fonte(s)' );
    ?>     
        
        <tr>
            <td class="subtitulodireita" width="190px">Data de Cadastro:</td>
            <td>
                <?php 
                    $data_de  = '';
                    $data_ate = '';
                ?>
                de: <input type="text" id="rstdtinclusao_de" name="rstdtinclusao_de" value="<?php echo $data_de;?>" size="15" maxlength="10" class="normal"> 
                &nbsp;
                até: <input type="text" id="rstdtinclusao_ate" name="rstdtinclusao_ate" value="<?php echo $data_ate;?>" size="15" maxlength="10" class="normal">
            </td>
            <td>&nbsp;</td>                
        </tr>
        <tr>
            <td class="subtitulodireita" width="190px">Data de Superação:</td>
            <td>
                <?php 
                    $data_de  = '';
                    $data_ate = '';
                ?>
                de: <input type="text" id="rstdtsuperacao_de" name="rstdtsuperacao_de" value="<?php echo $data_de;?>" size="15" maxlength="10" class="normal"> 
                &nbsp;
                até: <input type="text" id="rstdtsuperacao_ate" name="rstdtsuperacao_ate" value="<?php echo $data_ate;?>" size="15" maxlength="10" class="normal">
            </td>
            <td>&nbsp;</td>                
        </tr>
        
        <tr>
            <td class="subtituloDireita">Existe Ressalva ?</td>
            <td>
                <?php
                    $select_s = '';
                    $select_n = '';
                    $select_t = '';
                    switch ($_POST['rstflressalva']) {
                        case 'S':
                            $select_s = 'checked="checked"';
                            break;
                        case 'N':
                            $select_n = 'checked="checked"';
                            break;
                        case 'T':
//                        default:
                            $select_t = 'checked="checked"';
                            break;
                    }
                ?>
                <input type="radio" name="rstflressalva" id="rstflressalva_s" value="S" <?php echo $select_s; ?> > Sim
                <input type="radio" name="rstflressalva" id="rstflressalva_n" value="N" <?php echo $select_n; ?> > Não
                <input type="radio" name="rstflressalva" id="rstflressalva_t" value="T" <?php echo $select_t; ?> > Todas
            </td>
        </tr>
        
<tr>
	<td bgcolor="#CCCCCC"></td>
	<td bgcolor="#CCCCCC">
		<input type="button" value="Visualizar" onclick="exibeRelatorio('exibir');" style="cursor: pointer;"/>
		<input type="button" value="Visualizar XLS" onclick="exibeRelatorio('xls');" style="cursor: pointer;"/>
	</td>
</tr>
</table>

<script lang="javascript">
            
setTimeout(function(){
    
    jQuery('#rstdtinclusao_de').mask('99/99/9999');
    jQuery('#rstdtsuperacao_de').mask('99/99/9999');
    jQuery('#rstdtinclusao_ate').mask('99/99/9999');
    jQuery('#rstdtsuperacao_ate').mask('99/99/9999');
    
    var d = new Date();
    var data_hoje = d.toUTCString();
    
    jQuery("#rstdtinclusao_de").datepicker({
        numberOfMonths: 1,
        dateFormat: 'dd/mm/yy',
        showWeek: true,
        showAnim:'drop'
    });
    jQuery("#rstdtinclusao_ate").datepicker({
        numberOfMonths: 1,
        dateFormat: 'dd/mm/yy',
        showWeek: true,
        showAnim:'drop'
    });
    jQuery("#rstdtsuperacao_de").datepicker({
        numberOfMonths: 1,
        dateFormat: 'dd/mm/yy',
        showWeek: true,
        'showAnim':'drop'
    });
    jQuery("#rstdtsuperacao_ate").datepicker({
        numberOfMonths: 1,
        dateFormat: 'dd/mm/yy',
        showWeek: true,
        'showAnim':'drop'
    });
               
}, 500);

    function carregarMunicipio(estuf) {
        var td = $('#td_municipio');
        if (estuf != '') {
            var url = location.href;
            $.ajax({
                url: url,
                type: 'post',
                data: {ajax: 'municipio',
                    estuf: estuf},
                dataType: "html",
                async: false,
                beforeSend: function () {
                    divCarregando();
                    td.find('select option:first').attr('selected', true);
                },
                error: function () {
                    divCarregado();
                },
                success: function (data) {
                    console.info(data);
                    td.html(data);
                    divCarregado();
                }
            });
        } else {
            td.find('select option:first').attr('selected', true);
            td.find('select').attr('selected', true)
                .attr('disabled', true);
        }
    }

    formulario = document.getElementById('filtro');
    
    function exibeRelatorio(tipo){

        if(document.getElementById('prfid_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'prfid' ) );
        }
        if(document.getElementById('tooid_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'tooid' ) );
        }
        
        if(document.getElementById('esdid_ri_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'esdid_ri' ) );
        }
        
        if(document.getElementById('esdid_obr_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'esdid_obr' ) );
        }
        
        if(document.getElementById('estuf_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'estuf' ) );
        }
        
        if(document.getElementById('muncod_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'muncod' ) );
        }

        document.getElementById('pesquisa').value = tipo;

        formulario.target = 'RelatorioRestricoesInconformidades';
        var janela = window.open('obras2.php?modulo=relatorio/popupRelatorioRestricoesInconformidades&acao=A', 'RelatorioRestricoesInconformidades', 'status=no,menubar=no,toolbar=no,scrollbars=1,resizable=yes,width=780,height=460');

        janela.focus();

        formulario.submit();
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
</form>