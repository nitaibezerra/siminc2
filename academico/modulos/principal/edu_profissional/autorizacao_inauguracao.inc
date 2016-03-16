<?PHP
$_SESSION['academico']['entid'] = $_REQUEST['entidunidade'] ? $_REQUEST['entidunidade'] : $_SESSION['academico']['entid'];
    require_once APPRAIZ . 'includes/workflow.php';

    include_once APPRAIZ . "academico/modulos/principal/edu_profissional/autorizacaoInauguracaoController.php";

    if($_REQUEST['requisicao']){
        $_REQUEST['requisicao']($_REQUEST);
    }

    if($_REQUEST['requisicao_curso']){
        $_REQUEST['requisicao_curso']($_REQUEST);
    }

    if($_REQUEST['requisicao_fotos']){
        $_REQUEST['requisicao_fotos']($_REQUEST);
    }

    if($_REQUEST['request'] == 'download'){
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        if ($_REQUEST['arqid']){
            $file = new FilesSimec("fotosobra", array('ingid' => $resp_ingid), 'academico');
            $file->getDownloadArquivo($_REQUEST['arqid']);
        }
    }

    if ($_REQUEST['exibirfoto']) {
	echo '<img src="/slideshow/slideshow/verimagem.php?&arqid='.$_REQUEST['arqid'].'" width="800" height="600"/>';
	die;
    }

    include_once APPRAIZ . "includes/cabecalho.inc";

    echo "<br/>";

    //$abacod_tela = 57267;
    #$db->cria_aba($abacod_tela, $url, $parametros);
    monta_titulo("Autorização de Inauguração", "");

    $habilitado = 'S';
    $ingid = $_REQUEST['ingid'];
    $tipor = $_REQUEST['tipor'];

    if (!empty($ingid)) {
        $docid = pegarDocidInauguracao( $ingid );
        $dados = buscaDadosInauguracao($ingid);
    }
    
    #REGRAS: NO MOMENTO DE CARREGAR OS DADOS. PARA HABILITAR OS CAMPOS DA UNIDADE E DIRIGETE DE ACORDO COM AS OPÇÕES SELECIONADAS NO CAMPO INAUGURAÇÃO. 
    if( $dados['ingtipo'] == 'R' ){
        $hab = 'N';
        $obr = 'N';
    }else{
        $hab = 'S';
        $obr = 'S';
    }
?>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>
    <link rel="stylesheet" type="text/css" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>
    <link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css"></link>
    <link rel="stylesheet" type="text/css" href="../academico/css/modal_css_basic.css"/>

    <style>
        .imgExcluir {
            cursor: pointer;
            position: relative;
            z-index: 10;
            top: 0px;
            left: 0px;
            width: 18px;
        }

        .imgAbrirGaleria {
            float: left;
            width: 100px;
            height:122px;
            text-align:center;
            margin:10px;
            cursor: pointer;
        }

        .imgDowload {
            cursor: pointer;
            position: relative;
            z-index: 10;
            top: 0px;
            left: 0px;
        }

        .form_insert_cursos {
            margin-top: -14px;
            overflow-y: auto;
            float: right;
            width: 50%;
            height: 285;
        }

        .posicao_imagem {
            cursor: pointer;
            position: relative;
            top: -1px;
        }

        .posicao_texto {
            cursor: default;
            position: relative;
            top: -4px;
        }

        a:link, a:visited, a:hover, a:active {
            cursor: pointer;
            text-decoration: none;
        }
    </style>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
    <script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <script type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
    <script type="text/javascript" src="../academico/js/jquery.simplemodal.js"></script>

    <!-- PARA O USO DO FANCYBOX É NECESSÁRIO USAR O JQUEY 1.9.1 -->
    <script src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
    <script type="text/javascript">
	$jr = $.noConflict();
    </script>

    <link rel="stylesheet" type="text/css" href="/library/fancybox/jquery.fancybox.css" media="screen" />
    <link rel="stylesheet" type="text/css" href="/library/fancybox/helpers/jquery.fancybox-buttons.css" />
    <link rel="stylesheet" type="text/css" href="/library/fancybox/helpers/jquery.fancybox-thumbs.css" />

    <script type="text/javascript" src="/library/fancybox/jquery.fancybox.js"></script>
    <script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-buttons.js"></script>
    <script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-thumbs.js"></script>
    <script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-media.js"></script>

    <script type="text/javascript">
        $1_11(document).ready(function (){

            $1_11(window).unload(
                $1_11('.chosen-select').chosen()
            );

            $jr('.fancybox_img').fancybox({
		openEffect	: 'elastic',
                closeEffect	: 'elastic',
                helpers : {
                    title : {
    			type : 'inside'
                    },
                    thumbs : {
                        width	: 100,
                        height	: 50
                    }
                }
            });
        });

        function alterarInauguracao( id ){
            $('#tipor').val('A');
            $('#ingid').val(id);
            $('#formulario').submit();
        }

        function abrirFormulario(tipo, opc){
            $('#tipor').val('');

            if( tipo == 'C' ){
                if(opc == 'mais'){
                    $('#tr_tabala_cadastro').css("display", "");
                    $('#c_sinal_mais').css("display", "none");
                    $('#c_sinal_menos').css("display", "");

                    $('#tr_tabala_pesquisa').css("display", "none");
                    $('#p_sinal_mais').css("display", "");
                    $('#p_sinal_menos').css("display", "none");

                    $('#tb_cadastro_ing').css("display", "");
                }else{
                    $('#tr_tabala_cadastro').css("display", "none");
                    $('#c_sinal_mais').css("display", "");
                    $('#c_sinal_menos').css("display", "none");

                    $('#tr_tabala_pesquisa').css("display", "");
                    $('#p_sinal_mais').css("display", "none");
                    $('#p_sinal_menos').css("display", "");

                    $('#tb_cadastro_ing').css("display", "none");
                }
            }

            if( tipo == 'P' ){
                if(opc == 'mais'){
                    $('#tr_tabala_pesquisa').css("display", "");
                    $('#p_sinal_mais').css("display", "none");
                    $('#p_sinal_menos').css("display", "");

                    $('#tr_tabala_cadastro').css("display", "none");
                    $('#c_sinal_mais').css("display", "");
                    $('#c_sinal_menos').css("display", "none");

                    $('#tb_cadastro_ing').css("display", "none");
                }else{
                    $('#tr_tabala_pesquisa').css("display", "none");
                    $('#p_sinal_mais').css("display", "");
                    $('#p_sinal_menos').css("display", "none");

                    $('#tr_tabala_cadastro').css("display", "");
                    $('#c_sinal_mais').css("display", "none");
                    $('#c_sinal_menos').css("display", "");

                    $('#tb_cadastro_ing').css("display", "");
                }
            }
        }
        
        function abrirCadDirigente( opc ){
            if( opc == 'R' ){
                window.location.href ='academico.php?modulo=principal/dadosdirigentes&acao=A';
            }
            
            if( opc == 'U' ){
                var entid = $('#entidcampus').val();
                window.location.href ='academico.php?modulo=principal/dadosdirigentes&acao=C&tela=ing&entid='+entid;
            }
        }
        
        function abrirCadEndereco(){
            var opc = $('input[name=ingtipo]:checked').val();
            
            if( opc == 'R' ){
                window.location.href ='academico.php?modulo=principal/dadosentidade&acao=A';
            }
            
            if( opc == 'O' || opc == 'U' ){
                var entid = $('#entidcampus').val();
                window.location.href ='academico.php?modulo=principal/alterarCampus&acao=A&entid='+entid;
            }
        }

        function abrirGaleria(arqid){
            var url = "../includes/highslide/verimagem.php?newwidth=64&newheight=64&arqid="+arqid+"&_sisarquivo=academico"

            window.open(url,"imagem","width=850,height=600,resizable=yes");
        }

        function atualizaComboMunicipioInauguracao(estuf) {
            $.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=atualizaComboMunicipioInauguracao&estuf=" + estuf,
                async: false,
                success: function (resp) {
                    $('#td_combo_municipio').html(resp);
                }
            });
        }

        function atualizaComboUnidade( opc ){
            $('#edocep').val('');
            $('#edodsc').val('');
            $('#edocomplemento').val('');
            $('#edobairro').val('');
            $('#estuf').val('');
            $('#muncod').val('');

            $('#cadEnderecoEnt').css('display', 'none');

            $.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=atualizaComboUnidade&opc="+opc,
                async: false,
                success: function (resp){
                    if( opc == 'R' ){
                        buscaDirigentes('NI', 'R');

                        $('#entidcampus').attr("disabled", true);

                        buscarEndereco( 'NI', opc );
                    }else{
                        $('#entidcampus').attr("disabled", false);
                        $('#td_combo_unidade').html(resp);
                    }
                }
            });
        }

        function buscaDirigentes( entid, opc ){
            $('#ingdscreitor').attr('readonly', true);
            $('#ingnomedirigente').attr('readonly', true);

            if( opc == 'R'){
                $('#ingnomedirigente').attr('disabled', true);
                $('#ingnomedirigente').removeClass('obrigatorio');
                $('#ingnomedirigente').parent().find('img').css('display', 'none');
                $('#cadDirigUnidade').css('display', 'none');
            }else{
                $('#ingnomedirigente').attr('disabled', false);
                $('#ingnomedirigente').addClass('obrigatorio');
                $('#ingnomedirigente').parent().find('img').css('display', '');
            }

            $.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=buscaDirigentes&opc="+opc+"&entid="+entid,
                success: function (resp) {
                    var dados = $.parseJSON(resp);

                    if( opc == 'R' ){
                        if( trim(dados.entnome) == '' ){
                            $('#cadDirigReitor').css('display', '');
                        }else{
                            $('#cadDirigReitor').css('display', 'none');
                        }

                        $('#ingdscreitor').val('');
                        $('#ingnomedirigente').val('');
                        $('#ingdscreitor').val( dados.entnome );
                    }

                    if( opc == 'U' ){
                        if( trim(dados.entnome) == '' ){
                            $('#cadDirigUnidade').css('display', '');
                        }else{
                            $('#cadDirigUnidade').css('display', 'none');
                        }
                        $('#ingnomedirigente').val( dados.entnome );
                    } 
                }
            });
        }

        function buscarEndereco( entid, opc ){
            if(typeof(opc) == 'undefined'){
                opc = 'C';
                buscaDirigentes( 'NI', 'R' );
                buscaDirigentes( entid, 'U' );
            }

            $.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=buscarEndereco&opc="+opc+"&entid="+entid,
                success: function (resp) {
                    var dados = $.parseJSON(resp);
                    
                    var endcep = dados.endcep.substring(0, 5)+'-'+dados.endcep.substring(5, 8);
                    var endereco = dados.endlog +' nº: '+ dados.endnum;
                    
                    if( opc == 'R' ){
                        
                    }
                    
                    if( endcep == '' || endereco == '' || dados.endbai == '' || dados.estuf == '' || dados.muncod == '' ){
                        $('#cadEnderecoEnt').css('display', '');
                    }else{
                        $('#cadEnderecoEnt').css('display', 'none');
                    }

                    $('#edocep').val( endcep );
                    $('#edodsc').val( endereco );
                    $('#edocomplemento').val( dados.endcom );
                    $('#edobairro').val( dados.endbai );
                    $('#estuf').val( dados.estuf );
                    $('#muncod').val( dados.muncod );
                }
            });
        }

        function buscaListagemInauguracao( opc ){
            if(opc == 'P'){
                $('#formulario_pesquisa').submit();
            }
        }

        function cadastroNovoCurso(id) {
            $.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=cadastroNovoCurso&id=" + id,
                success: function (resp){
                    $('#form_insert_cursos').html(resp);
                    $('#form_insert_cursos').modal();
                    divCarregado();
                }
            });
        }

        function cadastrarNovaInauguracao( opc ){
            $.each($("input[type=text], input[type=hidden], select, textarea, input[type=radio], input[type=checkbox]"), function(i,v){
                if( $(this).attr('type') != 'radio' && $(this).attr('type') != 'checkbox' ){
                    if( $(this).attr('id') != 'entidies_dsc' && $(this).attr('id') != 'entidies' ){
                        $(v).val('');
                    }
                }
                if( $(this).attr('type') == 'radio' ){
                    $(v).attr('checked', false);
                }
                if( $(this).attr('type') == 'checkbox' ){
                    $(v).attr('checked', false);
                }
            });
            
            for(i=1; i<7; i++){
                $("#div_foto"+i).empty();                
            }

            $('#div_grid_cursos').html('');
            <?PHP unset($_SESSION['grid']['cursos']); ?>

            if( opc == 'P' ){
                $('#tr_tabala_pesquisa').css("display", "none");
                $('#p_sinal_mais').css("display", "");
                $('#p_sinal_menos').css("display", "none");

                $('#tr_tabala_cadastro').css("display", "");
                $('#c_sinal_mais').css("display", "none");
                $('#c_sinal_menos').css("display", "");

                $('#tb_cadastro_ing').css("display", "");
            }
        }

        function excluirImagemIng( arqid ){
            var confirma = confirm("Deseja realmente excluir a Foto?");
            if( confirma ){
                $('#arqid').val(arqid);
                $('#requisicao').val('excluirImagemIng');
                $('#formulario').submit();
            }
        }

        function excluirInauguracao( id ){
            $('#tipor').val('E');
            $('#ingid').val(id);

            if(confirm('Deseja excluir este registro')){
                $('#requisicao').val('excluirInauguracao');
                $('#formulario').submit();
            }
        }

        function excluirCursoGridTemp(id) {
            $.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao_curso=excluirCursoGridTemp&id=" + id + "&ingid=" + $('#ingid').val(),
                async: false,
                success: function (data) {
                    $("#div_grid_cursos").html(data);
                    window.close();
                }
            });
        }

        function fecheJanelaCorreta(){
            var tipor = $('#tipor').val();

            if( tipor == 'A' ){
                $('#tr_tabala_cadastro').css("display", "");
                $('#c_sinal_mais').css("display", "none");
                $('#c_sinal_menos').css("display", "");

                $('#tr_tabala_pesquisa').css("display", "none");
                $('#p_sinal_mais').css("display", "");
                $('#p_sinal_menos').css("display", "none");

                $('#tb_cadastro_ing').css("display", "");
            }else{
                $('#tr_tabala_cadastro').css("display", "none");
                $('#c_sinal_mais').css("display", "");
                $('#c_sinal_menos').css("display", "none");

                $('#tr_tabala_pesquisa').css("display", "");
                $('#p_sinal_mais').css("display", "none");
                $('#p_sinal_menos').css("display", "");

                $('#tb_cadastro_ing').css("display", "none");
            }
        }

        function imprimirInauguracao(id){
            var url = "academico.php?modulo=principal/edu_profissional/autorizacao_inauguracao_impr&acao=A&ingid="+id;
            window.open(url, "Autorização de Inauguração",'scrollbars=yes,height=600,width=1200,status=no,toolbar=no,menubar=no,location=no');
        }

        $(function () {
            $(".img_cad").change(function () {
                readURL(this, $(this).attr('name'), $(this).attr('accesskey'));
            });
        })

        function readURL(input, nome, legenda) {
            if (input.files && input.files[0]) {

                var reader = new FileReader();
                var idDiv = '#div_' + nome;

                reader.onload = function (e) {
                    var img = $('<img>');
                    img.attr('src', e.target.result);
                    img.attr('width', 100);
                    img.attr('class', 'img-thumbnail');
                    $(idDiv).html('');
                    img.appendTo(idDiv);
                    $('<p class="text-center">' + legenda + '</p>').appendTo(idDiv);
                }
                reader.readAsDataURL(input.files[0]);
            }
        }

        function salvarDadosFomulario(opc) {
            var erro;
            var campos = '';

            if (opc == 'C') {
                var inputs = $("#formulario_curso").find('.obrigatorio');
            } else if (opc == 'I') {
                var inputs = $("#formulario").find('.obrigatorio');
            }

            $.each(inputs, function (i, v) {
                if (($(this).attr('type') != 'radio') && ($(this).attr('type') != 'checkbox')) {
                    if ($(v).val() == '') {
                        erro = 1;
                        campos += '- ' + $(this).attr('title') + " \n";
                    }
                } else if ($(this).attr('type') == 'radio') {
                    var name = $(this).attr('name');
                    var value = $(this).attr('value');
                    var radio_box = $('input:radio[name=' + name + ']:checked');
                    if (!radio_box.val()) {
                        erro = 1;
                        if (value == 'O') {
                            campos += '- ' + $(this).attr('title') + " \n";
                        }
                    }
                } else if ($(this).attr('type') == 'checkbox') {
                    var name = $(this).attr('name');
                    var value = $(this).attr('value');
                    var check_box = $('input:checkbox[name=' + name + ']:checked');

                    if (!check_box.val()) {
                        erro = 1;
                        if (value == 'S') {
                            campos += '- ' + $(this).attr('title') + " \n";
                        }
                    }
                }
            });

            if (erro > 0) {
                alert('Existem "campos obrigatorios" vazios! São eles: \n' + campos);
                return false;
            }

            if (!erro) {
                if (opc == 'C') {
                    $('#requisicao_curso').val('salvarCurso');
                    $.ajax({
                        type: 'POST',
                        url: window.location.href,
                        data: $('#formulario_curso').serialize(),
                        async: false,
                        success: function (data) {
                            $("#div_grid_cursos").html(data);
                            window.close();
                        }
                    });
                }

                if(opc == 'I'){
                    $('#requisicao').val('salvarInauguracao');
                    $('#formulario').submit();
                }
            }
        }

        function voltarListaPessoa()
        {
            window.location = 'academico.php?modulo=inicio&acao=C';
        }
    </script>
    
<div class="row">
    <div class="col-md-2" style="position:static;">
        <?=cria_aba_2($abacod_tela, $url, $parametros);?>
    </div>
    
    <div class="col-md-9" style="position:static;">
        <?PHP
            // cria o cabeçalho padrão do módulo
            $autoriazacaoconcursos = new autoriazacaoconcursos();
            echo $autoriazacaoconcursos->cabecalho_entidade($_SESSION['academico']['entid']);
        ?>
        <div id="form_insert_cursos" style="display: none;"></div>

        <form method="POST" id="formulario_pesquisa" name="formulario_pesquisa">
            <table  bgcolor="#f5f5f5" align="center" class="tabela" border="0">
                <tr>
                    <td colspan="2" class="subTituloCentro" style="text-align: left;">
                        <img id="p_sinal_mais"  style="display: none; cursor: pointer; " src="../imagens/mais.gif" onclick="abrirFormulario('P', 'mais');">
                        <img id="p_sinal_menos" style="cursor: pointer;" src="../imagens/menos.gif" onclick="abrirFormulario('P', 'menos');">
                        <span style="margin-left: 6px; position: absolute; margin-top: -2px;">LISTAGEM DE INAUGURAÇÕES - PESQUISA.</span>
                    </td>
                </tr>
                <tr id="tr_tabala_pesquisa">
                    <td>
                        <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                            <tr>
                                <td class="SubTituloDireita" width="30%">Inauguração:</td>
                                <td colspan="3">
                                    <input type="radio" name="pesq_ingtipo" id="ingtipo_r" value="R"> Reitoria
                                    <input type="radio" name="pesq_ingtipo" id="ingtipo_u" value="U"> Unidade
                                    <input type="radio" name="pesq_ingtipo" id="ingtipo_o" value="O"> Obra nova
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita">Nome da Unidade/Obra:</td>
                                <td colspan="3">
                                    <?PHP
                                        $entidcampus = $dados['entidcampus'];

                                        $entid = $_SESSION['academico']['entid'];
                                        $orgid = $_SESSION['academico']['orgid'];

                                        if ($orgid == 1) {
                                            $funid = ACA_ID_CAMPUS;
                                        } else {
                                            $funid = ACA_ID_UNED;
                                        }
                                        $sql = "
                                            SELECT  e.entid AS codigo,
                                                    upper(e.entnome) AS descricao
                                            FROM entidade.entidade e2

                                            JOIN entidade.entidade e ON e2.entid = e.entid
                                            JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
                                            JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid
                                            JOIN entidade.endereco ed ON ed.entid = e.entid
                                            JOIN territorios.municipio mun ON mun.muncod = ed.muncod

                                            WHERE ea.entid = {$entid} AND e.entstatus = 'A' AND ef.funid = {$funid}

                                            ORDER BY e.entnome
                                        ";
                                        //$db->monta_combo('pesq_entidcampus', $sql, 'S', "Digite a Unidade/Obra...", '', '', '', 548, 'N', 'pesq_entidcampus', '', $entidcampus, 'Unidade/Obra', '', 'chosen-select');
                                        $db->monta_combo('pesq_entidcampus', $sql, 'S', "Digite a Unidade/Obra...", '', '', '', 548, 'N', 'pesq_entidcampus', '', $entidcampus, 'Unidade/Obra', '', '');
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita">Obra:</td>
                                <td colspan="3">
                                    <?PHP
                                        echo campo_texto('pesq_ingdscobra', 'N', 'S', 'Obra', 85, 100, '', '', '', '', 0, 'id="pesq_ingdscobra"', '', '', '', null);
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloCentro" colspan="3">
                                    <input type="button" id="pequisar" name="pequisar" value="Pesquisar" onclick="buscaListagemInauguracao('P');"/>
                                    <input type="button" id="cadastro" name="cadastro" value="Cadastrar Nova Inauguração" onclick="cadastrarNovaInauguracao('P');"/>
                                </td>
                            </tr>
                        </table>
                        <br>
                        <?PHP
                            buscaListagemInauguracao($_REQUEST);
                        ?>
                    </td>
                </tr>
             </table>
        </form>

        <form method="POST" id="formulario" name="formulario" enctype="multipart/form-data">
            <input type="hidden" id="requisicao" name="requisicao" value=""/>
            <input type="hidden" id="arqid" name="arqid" value=""/>
            <input type="hidden" id="tipor" name="tipor" value="<?=$tipor;?>"/>
            <input type="hidden" id="ingid" name="ingid" value="<?=$dados['ingid'];?>"/>
            <input type="hidden" id="edoid" name="edoid" value="<?=$dados['edoid'];?>"/>

            <table id="tb_cadastro_ing" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                <tr>
                    <td>
                        <span style="cursor: pointer" onclick="cadastrarNovaInauguracao('C');" title="Nova Inauguração" >
                            <img align="absmiddle" src="/imagens/gif_inclui.gif" width="11px"/> &nbsp; Cadastrar Nova Inauguração
                        </span>
                    </td>
                </tr>
            </table>

             <table  bgcolor="#f5f5f5" align="center" class="tabela" border="0">
                <tr>
                    <td colspan="2" class="subTituloCentro" style="text-align: left;">
                        <img id="c_sinal_mais"  style="cursor: pointer;" src="../imagens/mais.gif" onclick="abrirFormulario('C', 'mais');">
                        <img id="c_sinal_menos" style="display: none; cursor: pointer;" src="../imagens/menos.gif" onclick="abrirFormulario('C', 'menos');">
                        <span style="margin-left: 6px; position: absolute; margin-top: -2px;">CADASTRO DE NOVAS INAUGURAÇÕES.</span>
                    </td>
                </tr>
                <tr id="tr_tabala_cadastro">
                    <td>
                        <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                            <tr>
                                <td class="SubTituloCentro" style="text-align: left;" colspan="4"> Dados da Inauguração</td>

                                <!-- INICIO DO WORKFLOW -->
                                <?PHP if($ingid != '' ){ ?>
                                    <td rowspan="34" valign="top" align="center" width="100" id="td_workflow">
                                        <?PHP wf_desenhaBarraNavegacao( $docid, array('tpcid'=>$tpcid, 'ingid'=>$ingid ) );?>
                                        <br>
                                    </td>
                                <?PHP } ?>
                                <!-- FIM DO WORKFLOW -->

                            </tr>
                            <tr>
                                <td class="SubTituloDireita" width="30%">Inauguração:</td>
                                <td colspan="3">
                                    <?PHP
                                        switch( $dados['ingtipo'] ){
                                            case 'R':
                                                $checked_r = 'checked="checked"';
                                                break;
                                            case 'O':
                                                $checked_n = 'checked="checked"';
                                                break;
                                            case 'U':
                                                $checked_c = 'checked="checked"';
                                                break;
                                        }
                                    ?>
                                    <input type="radio" name="ingtipo" id="ingtipo_r" value="R" style="margin-left:0px;" class="obrigatorio" onclick="atualizaComboUnidade('R');" title="Inauguração" <?=$checked_r;?> > Reitoria
                                    <input type="radio" name="ingtipo" id="ingtipo_u" value="U" style="margin-left:5px;" class="obrigatorio" onclick="atualizaComboUnidade('U');" <?=$checked_c;?> > Unidade
                                    <input type="radio" name="ingtipo" id="ingtipo_o" value="O" style="margin-left:5px;" class="obrigatorio" onclick="atualizaComboUnidade('O');" <?=$checked_n;?> > Nova Obra
                                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita">Instituição:</td>
                                <td colspan="3">
                                    <?PHP
                                        $entidies_dsc = $db->pegaUm("SELECT entnome FROM entidade.entidade WHERE entid = {$_SESSION['academico']['entid']}");
                                        echo campo_texto('entidies_dsc', 'N', 'N', 'Nome da Instituição', 85, 100, '', '', '', '', 0, 'id="entidies_dsc"', '', $entidies_dsc, '', null);
                                    ?>
                                    <input type="hidden" name="entidies" id="entidies" value="<?= $_SESSION['academico']['entid']; ?>">
                                </td>
                            </tr>
                            <tr>
                                <td id="tr_combo_unidade" class="SubTituloDireita">Unidade:</td>
                                <td id="td_combo_unidade" colspan="3">
                                    <?PHP
                                        $entidcampus = $dados['entidcampus'];

                                        $entid = $_SESSION['academico']['entid'];
                                        $orgid = $_SESSION['academico']['orgid'];

                                        if ($orgid == 1) {
                                            $funid = ACA_ID_CAMPUS;
                                        } else {
                                            $funid = ACA_ID_UNED;
                                        }
                                        $sql = "
                                            SELECT  e.entid AS codigo,
                                                    upper(e.entnome) AS descricao
                                            FROM entidade.entidade e2

                                            JOIN entidade.entidade e ON e2.entid = e.entid
                                            JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
                                            JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid
                                            JOIN entidade.endereco ed ON ed.entid = e.entid
                                            JOIN territorios.municipio mun ON mun.muncod = ed.muncod

                                            WHERE ea.entid = {$entid} AND e.entstatus = 'A' AND ef.funid = {$funid}

                                            ORDER BY e.entnome
                                        ";
                                        $db->monta_combo('entidcampus', $sql, $hab, "Selecione a Unidade/Obra...", '', '', '', 548, $obr, 'entidcampus', '', $entidcampus, 'Unidade/Obra', '', '');
                                    ?>
                                </td>
                            </tr>
                             <tr>
                                <td class="SubTituloDireita">Obra:</td>
                                <td colspan="3">
                                    <?PHP
                                        $ingdscobra = $dados['ingdscobra'];
                                        echo campo_texto('ingdscobra', 'S', 'S', 'Obra', 85, 100, '', '', '', '', 0, 'id="ingdscobra"', '', $ingdscobra, '', null);
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="subTituloCentro" colspan="4" style="text-align: left;"> Dirigentes </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita">Reitor(a):</td>
                                <td colspan="3">
                                    <?PHP
                                        $ingdscreitor = $dados['ingdscreitor'];
                                        echo campo_texto('ingdscreitor', 'S', 'S', 'Nome do Reitor', 85, 100, '', '', '', '', 0, 'id="ingdscreitor"', '', $ingdscreitor, '', null);
                                    ?>
                                    <input type="button" id="cadDirigReitor" name="cadDirigReitor" value="Cadastrar Dirigente" style="display:none;" onclick="abrirCadDirigente('R');"/>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita">Dirigente da Unidade:</td>
                                <td id="dirigente" colspan="3">
                                    <?PHP
                                        $ingnomedirigente = $dados['ingnomedirigente'];
                                        echo campo_texto('ingnomedirigente', $hab, $obr, 'Nome do Dirigente da Unidade', 85, 100, '', '', '', '', 0, 'id="ingnomedirigente"', '', $ingnomedirigente, '', null);
                                    ?>
                                    <input type="button" id="cadDirigUnidade" name="cadDirigUnidade" value="Cadastrar Dirigente" style="display:none;" onclick="abrirCadDirigente('U');"/>
                                </td>
                            </tr>

                            <tr>
                                <td class="subTituloCentro" colspan="4" style="text-align: left;"> Data de início de Funcionamento:</td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita"> Instalações Provisórias:</td>
                                <td width="12%">
                                    <?PHP
                                        $ingdtinstprov = $dados['ingdtinstprov'];
                                        echo campo_data2('ingdtinstprov', 'S', $habilitado, 'Instalações Porvisórias', 'DD/MM/YYYY', '', '', $ingdtinstprov, '', '', 'ingdtinstprov');
                                    ?>
                                </td>
                                <td class="SubTituloDireita" width="13%"> Instalações Definitivas:</td>
                                <td width="35%">
                                    <?PHP
                                        $ingdtinstdef = $dados['ingdtinstdef'];
                                        echo campo_data2('ingdtinstdef', 'S', $habilitado, 'Instalações Definitivas', 'DD/MM/YYYY', '', '', $ingdtinstdef, '', '', 'ingdtinstdef');
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita"> Sugestão de data da inauguração: </td>
                                <td>
                                    <?PHP
                                        $ingsuginauguracao = $dados['ingsuginauguracao'];
                                        echo campo_data2('ingsuginauguracao', 'S', $habilitado, 'Instalações Porvisórias', 'DD/MM/YYYY', '', '', $ingsuginauguracao, '', '', 'ingsuginauguracao');
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita"> Justificativa para Data de Inauguração Sugerida: </td>
                                <td colspan="3">
                                    <?PHP
                                        $inginaugjustificativa = $dados['inginaugjustificativa'];
                                        echo campo_textarea('inginaugjustificativa', 'S', $habilitado, '', 100, 5, 500, '', 0, '', false, 'Justificativa', $inginaugjustificativa);
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="subTituloCentro" colspan="4" style="text-align: left;"> Endereço</td>
                            </tr>

                            <tr>
                                <td class="SubTituloDireita"> CEP:</td>
                                <td colspan="3">
                                    <?PHP
                                        $edocep = $dados['edocep'];
                                        echo campo_texto('edocep', 'S', 'S', 'CEP', 14, 9, '#####-###', '', 'left', '', 0, 'id="edocep"', '', $edocep, 'buscarEndereceCEP(this.value)', '', null);
                                    ?>
                                    <input type="button" id="cadEnderecoEnt" name="cadEnderecoEnt" value="Cadastrar Endereço" style="display:none;" onclick="abrirCadEndereco();"/>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita"> Logradouro:</td>
                                <td colspan="3">
                                    <?PHP
                                        $edodsc = $dados['edodsc'];
                                        echo campo_texto('edodsc', 'S', 'S', 'Logradouro', 85, 100, '', '', 'left', '', 0, 'id="edodsc"', '', $edodsc, null, '', null);
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita"> Complemento:</td>
                                <td colspan="3">
                                    <?PHP
                                        $edocomplemento = $dados['edocomplemento'];
                                        echo campo_texto('edocomplemento', 'N', 'S', 'Complemento', 85, 100, '', '', 'left', '', 0, 'id="edocomplemento"', '', $edocomplemento, null, '', null);
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita"> Bairro:</td>
                                <td colspan="3">
                                    <?PHP
                                        $edobairro = $dados['edobairro'];
                                        echo campo_texto('edobairro', 'S', 'S', 'Bairro', 85, 100, '', '', 'left', '', 0, 'id="edobairro"', '', $edobairro, null, '', null);
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita"> UF: </td>
                                <td colspan="3">
                                    <?PHP
                                        $estuf = $dados['estuf'];
                                        $sql = "
                                                 SELECT  estuf as codigo,
                                                         estuf as descricao
                                                 FROM territorios.estado
                                                 ORDER BY estuf
                                             ";
                                        $db->monta_combo('estuf', $sql, 'S', "Selecione...", 'atualizaComboMunicipioInauguracao', '', '', 300, 'S', 'estuf', '', $estuf, 'UF');
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita"> Município: </td>
                                <td id="td_combo_municipio" colspan="3">
                                    <?PHP
                                        $muncod = $dados['muncod'];
                                        $sql = "
                                                SELECT  muncod AS codigo,
                                                        mundescricao AS descricao
                                                FROM territorios.municipio
                                                ORDER BY descricao
                                            ";
                                        $db->monta_combo('muncod', $sql, 'S', "Selecione...", '', '', '', 300, 'S', 'muncod', '', $muncod, 'Município');
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="subTituloCentro" colspan="4" style="text-align: left;"> Cursos</td>
                            </tr>

                            <tr>
                                <td class="SubTituloDireita"> Listagem de Cursos:</td>
                                <td class="SubTituloDireita" colspan="3">
                                    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                                        <tr>
                                            <td>
                                                <span style="cursor: pointer" onclick="cadastroNovoCurso('');" title="Novo Curso">
                                                    <img align="absmiddle" src="/imagens/gif_inclui.gif" width="11px"/> &nbsp; Cadastrar Novo Curso
                                                </span>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td>
                                                <div id="div_grid_cursos">
                                                    <?PHP
                                                        if( !empty($ingid) ){
                                                            $acao = "'<center><img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirCursoGridTemp('||crsid||')\"></center>'";
                                                            $sql = "
                                                                SELECT  {$acao} AS acao,
                                                                        crsdsc,
                                                                        crsmatricula,
                                                                        tpcid,
                                                                        crsmodalidade,
                                                                        crsid
                                                                FROM academico.curso
                                                                WHERE ingid = {$ingid}
                                                                ORDER BY crsid
                                                            ";
                                                            $cursoCadastrados = $db->carregar($sql);
                                                            $arr = array();

                                                            if( is_array($cursoCadastrados) ){
                                                                foreach ($cursoCadastrados as $key => $curso) {
                                                                    $arr[$curso['crsid']] = $curso;
                                                                    unset($arr[$curso['crsid']]['crsid']);
                                                                    unset($cursoCadastrados[$key]['crsid']);
                                                                }
                                                                $_SESSION['grid']['cursos'] = $arr;

                                                                $cabecalho = array("Ação", "Curso", "Número de matrículas", "Tipo", "Regular ou Bf ou e-Tec");
                                                                $alinhamento = Array('center', '', '', '', '');
                                                                $tamanho = Array('5%', '', '', '', '');
                                                                $db->monta_lista_simples($cursoCadastrados, $cabecalho, 50, 10, 'N', 'center', 'N', false, $tamanho, $alinhamento);
                                                            }
                                                        }
                                                    ?>
                                                </div>
                                            </td>
                                        </tr>
                                    </table>
                                </td>
                            </tr>

                            <tr>
                                <td class="subTituloCentro" colspan="4" style="text-align: left;"> Pessoal</td>
                            </tr>

                            <tr>
                                <td class="SubTituloDireita">Docentes previstos:</td>
                                <td colspan="3">
                                    <?PHP
                                        $ingnumdocprevisto = $dados['ingnumdocprevisto'];
                                        echo campo_texto('ingnumdocprevisto', 'S', $habilitado, 'Docentes previstos', 35, 3, '###', '', '', '', 0, 'id="ingnumdocprevisto"', '', $ingnumdocprevisto, '', null);
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita">Docentes nomeados:</td>
                                <td colspan="3">
                                    <?PHP
                                        $ingnumdocnomeados = $dados['ingnumdocnomeados'];
                                        echo campo_texto('ingnumdocnomeados', 'S', $habilitado, 'Docentes nomeados', 35, 3, '###', '', '', '', 0, 'id="ingnumdocnomeados"', '', $ingnumdocnomeados, '', null);
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita">Téc. Adm. Previstos:</td>
                                <td colspan="3">
                                    <?PHP
                                        $ingnumtecprevisto = $dados['ingnumtecprevisto'];
                                        echo campo_texto('ingnumtecprevisto', 'S', $habilitado, 'Téc. Adm. Previstos', 35, 3, '###', '', '', '', 0, 'id="ingnumtecprevisto"', '', $ingnumtecprevisto, '', null);
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita">Téc. Adm. Nomeados:</td>
                                <td colspan="3">
                                    <?PHP
                                        $ingnumtecnomeados = $dados['ingnumtecnomeados'];
                                        echo campo_texto('ingnumtecnomeados', 'S', $habilitado, 'Téc. Adm. Nomeados', 35, 3, '###', '', '', '', 0, 'id="ingnumtecnomeados"', '', $ingnumtecnomeados, '', null);
                                    ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="SubTituloDireita">Investimento (custo da obra):</td>
                                <td colspan="3">
                                    <?PHP
                                        $ingvlrinvestimento = $dados['ingvlrinvestimento'];
                                        echo campo_texto('ingvlrinvestimento', 'S', $habilitado, 'Investimento (custo da obra)', 35, 22, '[.###],##', '', '', '', 0, 'id="ingvlrinvestimento"', '', $ingvlrinvestimento, '', null);
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita">Investimento Mobiliário e Equipamentos:</td>
                                <td colspan="3">
                                    <?PHP
                                        $ingvlrinvmobequip = $dados['ingvlrinvmobequip'];
                                        echo campo_texto('ingvlrinvmobequip', 'S', $habilitado, 'Investimento Mobilario e Equipamentos', 35, 22, '[.###],##', '', '', '', 0, 'id="ingvlrinvmobequip"', '', $ingvlrinvmobequip, '', null);
                                    ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="subTituloCentro" colspan="3" style="text-align: left;"> Relatório Fotográfico do Conjunto da obra</td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita"> Listagem de Fotos:</td>
                                <td class="SubTituloDireita" colspan="3">
                                    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                                        <tr>
                                            <?PHP
                                                if (empty($fotos)){
                                            ?>
                                                    <td width="60%">
                                                        <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                                                            <tr>
                                                                <td class="subtitulodireita">Foto panorâmica(conjunto da obra):</td>
                                                                <td>
                                                                    <input type="file" name="foto1" class="img_cad" accesskey="foto 01">
                                                                </td>
                                                            </tr>
                                                            <tr>
                                                                <td class="subtitulodireita">Foto da fachada:</td>
                                                                <td>
                                                                    <input type="file" name="foto2" class="img_cad" accesskey="foto 02">
                                                                </td>
                                                            </tr>
                                                            <tr>
                                                                <td class="subtitulodireita">Foto dos prédios(completo):</td>
                                                                <td>
                                                                    <input type="file" name="foto3" class="img_cad" accesskey="foto 03">
                                                                </td>
                                                            </tr>
                                                            <tr>
                                                                <td class="subtitulodireita">Foto de sala de aula:</td>
                                                                <td>
                                                                    <input type="file" name="foto4" class="img_cad" accesskey="foto 04">
                                                                </td>
                                                            </tr>
                                                            <tr>
                                                                <td class="subtitulodireita">Foto de laboratório:</td>
                                                                <td>
                                                                    <input type="file" name="foto5" class="img_cad" accesskey="foto 05">
                                                                </td>
                                                            </tr>
                                                            <tr>
                                                                <td class="subtitulodireita">Foto diversa:</td>
                                                                <td>
                                                                    <input type="file" name="foto6" class="img_cad" accesskey="foto 06">
                                                                </td>
                                                            </tr>
                                                        </table>
                                                    <td>
                                                    <div>
                                                        <?PHP
                                                            $fotos = carregarFotosObrasInaug($ingid);
                                                            if( is_array($fotos) ){
                                                                foreach ($fotos as $key => $foto) {
                                                                    $arrIdsFotos[] = filter_var($foto['arqdescricao'], FILTER_SANITIZE_NUMBER_INT);
                                                                }

                                                                for ($i = 1; $i <= 6; $i++){
                                                                    $key = array_search($i, $arrIdsFotos);
                                                                    if ($key !== false){
                                                                        $key++
                                                        ?>
                                                                        <div id="div_foto<?= $i; ?>" class="imgAbrirGaleria">
                                                                            <a id="fancybox_img_<?=$fotos[$key]['arqid'];?>" class="fancybox_img fancybox.ajax" data-fancybox-group="gallery" href="academico.php?modulo=principal/edu_profissional/autorizacao_inauguracao&acao=A&exibirfoto=1&arqid=<?=$fotos[$key]['arqid'];?>">
                                                                                <img class="img-thumbnail" style="height:55%; width:98% !important;" src="/slideshow/slideshow/verimagem.php?&arqid=<?=$fotos[$key]['arqid'];?>"/>
                                                                            </a>

                                                                            <div class="download" id="<?=$fotos[$key]['arqid']?>" style="margin-left:9px; float:left; margin-top:7px;">
                                                                                <a title="Download" href="academico.php?modulo=principal/edu_profissional/autorizacao_inauguracao&acao=A&request=download&arqid=<?=$fotos[$key]['arqid'];?>">
                                                                                    <img class="imgDowload" src="../imagens/salvar.png" border=0 title="Download da Imagem"/>
                                                                                </a>
                                                                            </div>

                                                                            <div style="width:57px; float:left; margin-top:7px;">
                                                                                <?=$fotos[$key]['arqdescricao']?>
                                                                            </div>

                                                                            <div id="<?=$fotos[$key]['arqid']?>" style="float:left; margin-top:7px;">
                                                                                <img class="imgExcluir" src="../imagens/excluir_2.gif" border="0" title="Excluir" onclick="excluirImagemIng('<?=$fotos[$key]['arqid'];?>');"/>
                                                                            </div>
                                                                        </div>
                                                        <?PHP
                                                                    }else{
                                                        ?>
                                                                        <div id="div_foto<?= $i; ?>" class="imgAbrirGaleria"></div>
                                                        <?PHP
                                                                    }
                                                                }
                                                            }else{
                                                        ?>
                                                                <div id="div_foto1" class="imgAbrirGaleria"></div>
                                                                <div id="div_foto2" class="imgAbrirGaleria"></div>
                                                                <div id="div_foto3" class="imgAbrirGaleria"></div>
                                                                <div id="div_foto4" class="imgAbrirGaleria"></div>
                                                                <div id="div_foto5" class="imgAbrirGaleria"></div>
                                                                <div id="div_foto6" class="imgAbrirGaleria"></div>
                                                        <?PHP
                                                            }
                                                        ?>
                                                    </div>
                                                </td>
                                            <?PHP } ?>
                                        </tr>
                                    </table>
                                </td>
                            </tr>
                            <tr>
                                <td class="subTituloCentro" colspan="3" style="text-align: left;"> Breve Histórico da Unidade</td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita"> Descrição:</td>
                                <td colspan="3">
                                    <?PHP
                                        echo campo_textarea('inghistoricound', 'S', $habilita, '', 100, 5, 999, '', 0, '', false, 'Breve Histórico da Unidade', $dados['inghistoricound']);
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="subTituloCentro" colspan="3" style="text-align: left;"> Informações do Município</td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita"> Descrição:</td>
                                <td colspan="3">
                                    <?PHP
                                        echo campo_textarea('inginfgeral', 'S', $habilitado, '', 100, 5, 999, '', 0, '', false, 'Informações do Município', $dados['inginfgeral']);
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="subTituloCentro" colspan="3" style="text-align: left;"> Informações gerais sobre as condições da obra</td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita"> Descrição:</td>
                                <td colspan="3">
                                    <?PHP
                                        echo campo_textarea('inginfcondobra', 'S', $habilitado, '', 100, 5, 999, '', 0, '', false, 'Informações gerais sobre as condições da obra', $dados['inginfcondobra']);
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="subTituloCentro" colspan="3" style="text-align: left;"> Observações:</td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita"> Descrição:</td>
                                <td colspan="3">
                                    <?PHP
                                        echo campo_textarea('ingiobservacoes', 'S', $habilitado, '', 100, 5, 999, '', 0, '', false, 'Observações', $dados['ingiobservacoes']);
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloCentro" colspan="4">
                                    <input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarDadosFomulario('I');"/>
                                    <input type="button" id="voltar" name="voltar" value="Voltar" onclick="voltarListaPessoa();"/>
                                </td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
        </form>
    </div>
</div>
    
<script type="text/javascript">
    fecheJanelaCorreta();
</script>