<?php

$_SESSION['obras2']['obrid'] = $_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid'];
$obrid = $_SESSION['obras2']['obrid'];


if($_REQUEST['form'] == '1' && $_REQUEST['tipo_relatorio'] == 'xls'){
    ini_set("memory_limit", "7000M");
    montaLista($_REQUEST['tipo_relatorio']);
    exit;
}


if(empty($_SESSION['obras2']['empid'])){
    $objObr = new Obras($obrid);
    $empid =  $objObr->empid;
    $_SESSION['obras2']['empid'] = $empid;
}else{
    $empid =  $_SESSION['obras2']['empid'];
}

if($_GET['acao'] != 'L'){

switch ( $_REQUEST['requisicao'] ){
	case 'apagar': 
		$restricao = new Restricao( $_REQUEST['rstid'] );
            
		$restricao->rststatus = 'I';		
		$restricao->salvar();
		
		if ( ($restricao->usucpfsuperacao || $restricao->rsqid || $restricao->qtsid) && possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC) ) ){
			
			$procedencia = ( $restricao->rsqid || $restricao->qtsid ) ? 'Questionário (cadastro automático)' : 'Cadastro Manual';
			$superado    = ( $restricao->usucpfsuperacao ) ? 'Sim' : 'Não';
			$dtSuperado  = ( $restricao->rstdtsuperacao ) ? 'Em: ' . formata_data( $restricao->rstdtsuperacao ) : '';
			$item        = ( $restricao->rstitem == 'R' ) ? 'Restrição' : 'Inconformidade';
			
			$faseRestricao = new FaseRestricao( $restricao->fsrid );
			$tipoRestricao = new TipoRestricao( $restricao->tprid );
			
			$regAtividade                 = new RegistroAtividade();
			$arDado                       = array();
			$arDado['obrid']              = $obrid;
			$arDado['usucpf']             = $_SESSION['usucpf'];
			$arDado['rgaautomatica']      = 't';
			$arDado['rgadscsimplificada'] = 'RESTRIÇÃO E/OU INCONFORMIDADE APAGADA';
			$arDado['rgadsccompleta']     = "CPF: {$_SESSION['usucpf']}
                                                         Quando: " . date('d/m/Y') . "
                                                         Procedência: {$procedencia}
                                                         Item: {$item}
                                                         Situação da Obra na Ocorrência: {$faseRestricao->fsrdsc}
                                                         Tipo: {$tipoRestricao->tprdsc}
                                                         Descrição: {$restricao->rstdsc}
                                                         Previsão da Providência: " . formata_data( $restricao->rstdtprevisaoregularizacao ) . "
                                                         Providência: {$restricao->rstdscprovidencia}
                                                         Superado: {$superado} {$dtSuperado}";
			
			$regAtividade->popularDadosObjeto( $arDado )
						 ->salvar(); 
						        
		}elseif ( ($restricao->usucpfsuperacao || $restricao->rsqid || $restricao->qtsid) ){
			$db->rollback();
			die("<script>
                                    alert('Operação não realizada!\n Usuário sem permissão para realização da operação.'); 
                                    window.location = '?modulo=principal/listaRestricao&acao={$_GET['acao']}';
                             </script>");
		}
		
		$db->commit();
		die("<script>
                            alert('Operação realizada com sucesso!'); 
                            window.location = '?modulo=principal/listaRestricao&acao={$_GET['acao']}';
                     </script>");
}

if( $_GET['acao'] != 'V' ){
	include  APPRAIZ."includes/cabecalho.inc";
	echo "<br>";
	if ( $_GET['acao'] == 'A' ){
		// empreendimento || obra || orgao
		verificaSessao( 'empreendimento' );
		
		$db->cria_aba(ID_ABA_EMP_CADASTRADO,$url,$parametros);
		
		$empreendimento = new Empreendimento( $empid );
		$empreendimento->montaCabecalho();
		
	}else{
		// empreendimento || obra || orgao
		verificaSessao( 'obra' );
		
		if( $_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ){
			$db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros);
		}else{
			$db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros);
		}
		
		echo cabecalhoObra($obrid);
				
	}
	
	$habilitado = true;
	$habilita   = 'S';
	
}
else{
	// empreendimento || obra || orgao
	verificaSessao( 'obra' );
?>	
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<?php
	$db->cria_aba($abacod_tela,$url,$parametros);
	echo cabecalhoObra($obrid);
//	criaAbaVisualizacaoObra();
//	$somenteLeitura = true;
	$habilitado = false;
	$habilita   = 'N';
    $habilitada_insercao = false;
}
    if( possui_perfil( array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CALL_CENTER, PFLCOD_CONSULTA_TIPO_DE_ENSINO) ) ){
        $habilitado = false;
        $habilita = 'N';
    }


if( possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC) ) ){
	$habilitada_insercao = true;

}

monta_titulo($titulo_modulo, '</td></tr><tr><td style="background: #f00; color: #fff"><img src="/imagens/atencao.png" /> Somente as RESTRIÇÕES não superadas impedem o repasse de recursos. Inconformidades, caso não sejam superadas, serão analisadas na prestação de contas ao final da obra. A superação das inconformidades está condicionada à correção, conforme projeto pactuado com o FNDE.');
?> 
        
<script type="text/javascript">
    
function cadastroNovoDocid(rstid){
        var r1 = window.confirm('Você deseja cadastrar um Documento para a restrição '+rstid+' ?');
        if(r1){
//            alert('Cadastrando...');
            var url = '?modulo=principal/listaRestricao&acao=L&rstid='+rstid+'&novoDoc=true';
            janela = window.open(url, 'inserirRestricao', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' ); 
        }else{
            return false;
        }
    }
    
</script>
        
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
    <tr>
        <td colspan="2" align="center">
            <?php if($habilitada_insercao && $habilitado && !possui_perfil( array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_GESTOR_UNIDADE, PFLCOD_EMPRESA_MI_FISCAL, PFLCOD_EMPRESA_MI_GESTOR))): ?>
                <input type="button" 
                       value="Inserir Nova Restrição e Inconformidade" 
                       style="padding: 1px; padding-left: 20px; padding-right: 20px; margin-top:3px; margin-bottom:3px;" 
                       onclick="janela = window.open('?modulo=principal/cadRestricao&acao=<?php echo $_GET['acao'] ?>', 'inserirRestricao', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' ); janela.focus();"/>
            <?php endif; ?>
        </td>
    </tr>
    <tr><td colspan="2" style="text-align: center"></td></tr>
    <tr>
        <td style="text-align: left; width: 70px">
            <span><b>Providência</b> : </span>
        </td>
        <td style="text-align: left">
            <span><img style=" vertical-align:middle" src="/imagens/0_alerta.png" title="Aguardando Providência ou Aguardando Correção" /> Aguardando Providência ou Aguardando Correção | </span>
            <span><img style=" vertical-align:middle" src="/imagens/0_inativo.png" title="Aguardando Correção ou Aguardando Correção há mais de 15 dias" /> Aguardando Providência ou Aguardando Correção há mais de 15 dias | </span>
            <span><img style=" vertical-align:middle" src="/imagens/0_ativo.png" title="Aguardando Análise FNDE"/> Aguardando Análise FNDE | </span>
            <span><img style=" vertical-align:middle" src="/imagens/0_concluido.png" title="Superada" /> Superada | </span>
            <span><img style=" vertical-align:middle" src="/imagens/0_concluido_2.png" title="Justificada" /> Justificada | </span>
            <span><img style=" vertical-align:middle" src="/imagens/0_inexistente.png" title="Cancelada" /> Cancelada </span>
        </td>
    </tr>
</table>
        
<?php
$restricao = new Restricao();
if ( $_GET['acao'] == 'A' ){
	$param = array('empid' => $_SESSION['obras2']['empid']);
}else{
	$param = array('obrid' => $_SESSION['obras2']['obrid']);
}

if ( $habilitado == false ){
	$param['block_imgs_acao'] = true;
}else{
	$param['block_imgs_acao'] = false;    
}
$sql 	   = $restricao->listaSql( $param );
//    ver(simec_htmlentities($sql), d);
$cabecalho = array("Ação", "Providência", "ID Item", "Item", "Fase","Tipo", "Data da Inclusão", "Descrição", "Providência", "Previsão da Providência", "Criado Por", "Superação", "Situação Atual", "Superado Por", "Ressalva", "Último Tramite", "Data do Último Tramite");
$db->monta_lista($sql,$cabecalho,100,5,'N','center','N', 'N');

?>
<script type="text/javascript">
<!--
function abreHistoricoRest( rstid ){
	janela = window.open('?modulo=principal/popUpHistoricoRest&acao=A&rstid=' + rstid, 'Histórico Restrição', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' );
	janela.focus();
}

function alterarRest( rstid ){
	janela = window.open('?modulo=principal/cadRestricao&acao=<?php echo $_GET['acao'] ?>&rstid=' + rstid, 'inserirRestricao', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' ); 
	janela.focus();
}


function excluirRest( rstid ){
        <?
        $objObras = new Obras($obrid);
        $blockEdicao = $objObras->verificaObraVinculada();
        if($blockEdicao){
            echo 'var blockEdicao = true;';
        }else{
            echo 'var blockEdicao = false;';
        }
        ?>
                
	if ( confirm('Deseja apagar esta restrição?') ){
                if(blockEdicao){
                    alert('Você não pode editar os dados da Obra Vinculada.');
                }else{
                    location.href = '?modulo=principal/listaRestricao&acao=<?php echo $_GET['acao'] ?>&rstid=' + rstid + '&requisicao=apagar';
                }
	}
}
//-->
</script>

<?php

//end if $_GET['acao'] != 'L'
}
elseif ($_GET['acao'] == 'L'){
    
    if($_GET['novoDoc'] == true){
       atualizaDocidNullRestricao($_GET['rstid']); 
    }
    
    ?>
        
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
        
    <?php
    
    include  APPRAIZ."includes/cabecalho.inc";
    print '<br/>';
    $titulo_modulo    = 'Lista de Restrições e Inconformidades';
    $subtitulo_modulo = 'Pesquisa dos registros de todas as Obras';
    monta_titulo($titulo_modulo, $subtitulo_modulo);
    
    //Fluxo de Restrição/Inconformidade
    $tpdid = TPDID_RESTRICAO_INCONFORMIDADE;
    
    ?>
    
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    <script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
    <script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>

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
        var formulario = document.formulario;
        var tipo_relatorio = tipo;
        prepara_formulario();
        document.getElementById('tipo_relatorio').value = tipo_relatorio;
        formulario.submit();
        document.getElementById('tipo_relatorio').value = '';
    }
    
    function alterarRestricao(rstid, obrid, empid){
        var url = '?modulo=principal/cadRestricao&acao=O&rstid='+rstid+'&obrid='+obrid+'&empid='+empid;
        janela = window.open(url, 'inserirRestricao', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' ); 
        janela.focus();
    }
    
    function cadastroNovoDocid(rstid){
        var r1 = window.confirm('Você deseja cadastrar um Documento para a restrição '+rstid+' ?');
        if(r1){
//            alert('Cadastrando...');
            var url = '?modulo=principal/listaRestricao&acao=L&rstid='+rstid+'&novoDoc=true';
            janela = window.open(url, 'inserirRestricao', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' ); 
        }else{
            return false;
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
    
<form name="formulario" id="formulario" action="" method="post">

    <input type="hidden" name="form" value="1" /> 
    <input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value="" /> 
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
        <tr>
            <td class="subtituloDireita">Nome da Obra/ID:</td>
            <td>
                <?php 
                    $val = (!empty($_POST['obrbuscatexto'])) ? $_POST['obrbuscatexto'] : '';
                    echo campo_texto('obrbuscatexto', 'N', 'S', '', 70, 80, '', '', '', '', '', 'id="obrbuscatexto"', '', $val); 
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Esfera:</td>
            <td>
                <?php
                $sql = Array(Array('codigo' => 'E', 'descricao' => 'Estadual'),
                    Array('codigo' => 'M', 'descricao' => 'Municipal'));
                $db->monta_combo('empesfera', $sql, 'S', 'Selecione...', '', '', '', 200, 'N', 'empesfera');
                ?>
            </td>
        </tr>
                <?php
                    $tipologiaObra = new TipologiaObra();
                    $param = array("orgid" => $_SESSION['obras2']['orgid']);
                    $val = (!empty($_POST['tpoid'])) ? $_POST['tpoid'] : '';
                    mostrarComboPopup('Tipologia da Obra', 'tpoid', $tipologiaObra->listaCombo($param, true), $tipologiaObra->listaComboMarcados($_REQUEST[tpoid]), 'Selecione a(s) Tipologia(a) da(s) Obra(s)');
//                    $db->monta_combo("tpoid", $tipologiaObra->listaCombo($param), "S", "Todas", "", "", "", 200, "N", "tpoid", false, $val);
                ?>
     
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
                $val = (!empty($_POST['tprid'])) ? $_POST['tprid'] : '';

                $sql = " SELECT tprid as codigo, tprdsc as descricao 
                         FROM obras2.tiporestricao
                         WHERE tprstatus = 'A' 
                         ORDER BY tprid";
                $db->monta_combo("tprid", $sql, "S", "Todos", "", "", "", "200", "N", "tprid", false, $val);
                ?>
            </td>
        </tr>
        <tr>
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
                de: <input type="text" id="rstdtinclusao_de" name="rstdtinclusao_de" value="<?php echo $data_de;?>" size="15" maxlength="10" class="normal" > 
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
            <td colspan="2">
                <center>
                    <input type="button" id="btn_filtrar" name="btn_filtrar" value="Filtrar"      onclick="javascript:getLista('visual');"/>
                    <input type="button" id="btn_excel"   name="btn_excel"   value="Exportar XLS" onclick="javascript:getLista('xls');"/>
                </center>
            </td>
        </tr>
    </table>
    
</form>

    <table align="center" class="tabela">
        <tr>
            <td style="text-align: left; width: 70px">
                <span><b>Providência</b> : </span>
            </td>
            <td style="text-align: left">
                <span><img style=" vertical-align:middle" src="/imagens/0_alerta.png" title="Aguardando Providência ou Aguardando Correção" /> Aguardando Providência ou Aguardando Correção | </span>
                <span><img style=" vertical-align:middle" src="/imagens/0_inativo.png" title="Aguardando Correção ou Aguardando Correção há mais de 15 dias" /> Aguardando Providência ou Aguardando Correção há mais de 15 dias | </span>
                <span><img style=" vertical-align:middle" src="/imagens/0_ativo.png" title="Aguardando Análise FNDE"/> Aguardando Análise FNDE | </span>
                <span><img style=" vertical-align:middle" src="/imagens/0_concluido.png" title="Superada" /> Superada | </span>
                <span><img style=" vertical-align:middle" src="/imagens/0_concluido_2.png" title="Justificada" /> Justificada | </span>
                <span><img style=" vertical-align:middle" src="/imagens/0_inexistente.png" title="Cancelada" /> Cancelada </span>
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

</script>
    
<?php

if($_REQUEST['form'] == '1' || ( ($_GET['acao'] == 'L') AND possui_perfil(array(PFLCOD_GESTOR_UNIDADE, PFLCOD_SUPERVISOR_UNIDADE)) ) ){
    montaLista($_REQUEST['tipo_relatorio']);
}


}// END - elseif ($_GET['acao'] == 'L')

function atualizaDocidNullRestricao($rstid){
    $restricao = new Restricao();
    $resp = $restricao->atualizaDocidNullRetricao($rstid);
    
    if($resp){
        die("<script>
                alert('Documento atualizado com sucesso.'); 
                window.parent.opener.getLista('visual');
                window.close();
             </script>");
    }else{
        die("<script>
                alert('Documento não pode ser atualizado.'); 
                window.close();
             </script>");
    }
}

function montaLista($tipo){
    global $db;
    $restricao = new Restricao();
    $sql       = $restricao->getDadosListaRestricao('sql', $tipo);
//    ver($sql,d);
    $cabecalho = array( "Ação", "ID Obra", "ID Item", "Item", "Providência", "Situação", "Estado", "Município", "Esfera", "Fase", "Tipo",
                        "Nome da Obra", "Situação Obra", "Data Cadastro", "Criado por", "Previsão da Providência", "Superação", "Superado por", "Último Tramite", "Data do Último Tramite");

    if($tipo == 'xls'){

        $dados = $db->carregar($sql);

        $dados = (!$dados) ? array() : $dados;

        foreach ($dados as $key => $value){
            $historico = explode(';', $value['historico']);
            $dados[$key] = $dados[$key] + $historico;
            unset($dados[$key]['historico']);

            $contador[] = count($historico);

        }
        //ver(d,$contador);
             $i = 1;
        if($contador) {
            while ($i <= (max($contador) / 3 + 1)) {
                $cabecalho_extra[] = "Data {$i}";
                $cabecalho_extra[] = "Ação {$i}";
                $cabecalho_extra[] = "Usuário {$i}";
                $i++;
            }

        }

        $cabecalho = array( "ID Obra", "ID Item", "Item", "Situação", "Estado", "Município", "Esfera", "Fase", "Tipo", "Tipologia",
                                "Nome da Obra", "Situação","Data Cadastro", "Criado por", "Previsão da Providência", "Superação", "Superado por", "Usuário da Operação", "Data da Última Tramitação","Descrição","Providência","Divisão","Item");
        ini_set("memory_limit", "7000M");

        if(!empty($cabecalho_extra)){

        $novo_cabecalho = array_merge($cabecalho,$cabecalho_extra);
        }
        else{

             $novo_cabecalho = $cabecalho;
        }

        $db->sql_to_xml_excel ($dados, 'ListadeRestriçãoeInconformidade', $novo_cabecalho);
    }else{
//        ver($sql,d);
        $db->monta_lista($sql, $cabecalho, 30, 10,'N','center', 'N', 'N');
    }

}

?>