<?PHP
    $_SESSION['academico']['entid'] = $_REQUEST['entidunidade'] ? $_REQUEST['entidunidade'] : $_SESSION['academico']['entid'];
    
    if(!$_SESSION['academico']['entid']){
    	die ( "
            <script>
                window.location='academico.php?modulo=inicio&acao=C';
            </script>" );
    }
    
    if( $_REQUEST['requisicao']  == 'excluirDirigente' ){
        apagaDadosDirigentes( $_REQUEST['ex_entid'], $_REQUEST['ex_funid'], $_REQUEST['ex_mcsid'] );
    }

    if($_REQUEST['requisicao'] == 'exibirListaDirigentes' ) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    if($_REQUEST['requisicao'] == 'selecionarDirigenteCadastro' ) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    if($_REQUEST['download'] == 'S'){
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        $file = new FilesSimec();
        $arquivo = $file->getDownloadArquivo($_GET['arqid']);
        exit;
    }

    require_once APPRAIZ . "includes/cabecalho.inc";
    echo '<br/>';

    if ( $_REQUEST['acao'] == 'A' ){
        $entid = $_SESSION['academico']['entid'];        
    }else if ( $_REQUEST['acao'] == 'C' ){

        if( $_REQUEST['tela'] == 'ing' ){
            $_SESSION['academico']['entidcampus'] = $_REQUEST['entid'];

            $_SESSION['academico']['entidadenivel'] = 'campus';
        }

        $entid = $_SESSION['academico']['entidcampus'];
    }

    //Faz o redirecionamento caso não tenha entidade selecionada.
    verificaExistenciaEntidades();
    
    #MONTA AS ABAS
    if( $_SESSION['academico']['entidadenivel'] != 'campus' ){
        $abacod_tela = '57135';
        $url = "academico.php?modulo=principal/dadosdirigentes&acao=A";
    }else{
        $abacod_tela = '57268';
        $url = "academico.php?modulo=principal/dadosdirigentes&acao=C";
    }

    monta_titulo( "Dados dos Dirigentes", "");

    $autoriazacaoconcursos = new autoriazacaoconcursos();
    $unidade = $autoriazacaoconcursos->buscaentidade( $entid );
    $consulta = false;
    
    // cria o cabeçalho padrão do módulo
    $autoriazacaoconcursos = new autoriazacaoconcursos();
    
    $perfilNotBloq = array(
        //PERFIL_IFESCADBOLSAS,
        PERFIL_CADASTROGERAL,
        PERFIL_MECCADBOLSAS,
        PERFIL_MECCADCURSOS,
        PERFIL_MECCADASTRO,
        PERFIL_ADMINISTRADOR,
        PERFIL_REITOR,
        PERFIL_ALTA_GESTAO,
        PERFIL_ASSESSORIA_ALTA_GESTAO,
        PERFIL_INTERLOCUTOR_INSTITUTO,
        PERFIL_PROREITOR
    );

    if($_REQUEST['acao'] == 'C'){
        $perfilNotBloq[] = PERFIL_INTERLOCUTOR_CAMPUS;
        $perfilNotBloq[] = PERFIL_DIRETOR_CAMPUS;
    }

    #VERIFICANDO A SEGURANÇA
    $permissoes = verificaPerfilAcademico($perfilNotBloq);
    validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['academico']['orgid']);
    #FIM SEGURANÇA

    $consulta = $bloqueado = $permissoes['gravar'] ? false : true;
    $consulta = inArrayMultiple(array(PERFIL_DIRETOR_CAMPUS,PERFIL_INTERLOCUTOR_CAMPUS),pegaPerfilGeral($_SESSION['usucpf']));
    
    if( $_SESSION['academico']['entidadenivel'] != 'campus' ){
        if( $_SESSION['academico']['entid'] != '' ){
            $v_entid = $_SESSION['academico']['entid'];
        }else{
            $_SESSION['academico']['campus_und'] = $unidade;
            $v_entid = $unidade;
        }

        $AND = "AND (mb.mcssituacao = 't' OR mcssituacao IS NULL) AND mb.tpmid IS NULL  AND fen.funid IN (21,40)";
    }else{
        $v_entid    = $_SESSION['academico']['entidcampus'];
        
        $AND = "AND (mb.mcssituacao = 't' OR mcssituacao IS NULL) AND mb.tpmid IS NULL  AND fen.funid IN (24,40)";
    }

    if( $v_entid != '' ){

        $acao = "
            <a href=\"academico.php?modulo=principal/dadosdirigentes&acao=A&download=S&arqid='|| mb.arqid ||'\">
                <img src=\"../imagens/anexo.gif\" border=\"0\">
            </a>
        ";

        $sql = "
            SELECT  fun.funid,
                    fun.fundsc,
                    mb.mcsid AS mcsid,
                    ent.entnome,
                    ent.entid,
                    mcsvigenciainicial,
                    mcsvigenciafinal,

                    CASE WHEN mb.mcssituacao = 't'
                        THEN 'Ativo'
                        ELSE CASE WHEN mb.mcssituacao = 'f'
                                THEN 'Inativo'
                                ELSE ''
                             END
                    END AS mcssituacao,

                    CASE WHEN mb.arqid > 0
                        THEN '{$acao}'
                        ELSE CASE WHEN mb.mcsid IS NULL
                                THEN ''
                                ELSE 'Não Anexado'
                             END
                    END AS arqid

            FROM entidade.funcaoentidade AS fen

            JOIN entidade.funcao AS fun ON fun.funid = fen.funid
            LEFT JOIN academico.membroconselho AS mb ON mb.entid = fen.entid
            LEFT JOIN entidade.entidade AS ent ON ent.entid = fen.entid
            LEFT JOIN entidade.funentassoc AS fua ON fua.fueid = fen.fueid AND fua.feaid = mb.feaid
            LEFT JOIN entidade.funentassoc AS fua2 ON fua2.feaid = mb.feaid

            WHERE fua.entid = {$v_entid} {$AND}

            ORDER BY fun.funid
        ";
        $dirigentes = $db->carregar($sql);
    }

    if( $_SESSION['academico']['entidadenivel'] != 'campus' ){
        $entid = $_SESSION['academico']['entid'];
        $titulo = "CONSELHO SUPERIOR";
    }else{
        $entid = $_SESSION['academico']['entidcampus'];
        $titulo = "CONSELHO DO CAMPUS";
    }
?>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>
    <link rel="stylesheet" type="text/css" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>
    <link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css"></link>
    <link rel="stylesheet" type="text/css" href="../academico/css/modal_css_basic.css"/>

    <style>
        .simplemodal-container{
            height: 135px !important;
        }
    </style>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
    <script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <script type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
    <script type="text/javascript" src="../academico/js/jquery.simplemodal.js"></script>
    <link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">

     <script type="text/javascript">

        function abredadoscampus( entid ){
            return window.location = '?modulo=principal/alterarCampus&acao=A&entid=' + entid;
	}

         //#CARREGAR LISTA FILHO
        function carregarListaDirigentes(idImg, entid){
            var img     = $( '#'+idImg );
            var tr_nome = 'listaDirigentes_'+ entid;
            var td_nome = 'trA_'+ entid;

            if( $('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == "" ){
                $('#'+td_nome).html('Carregando...');
                img.attr ('src','../imagens/menos.gif');
                exibirListaDirigentes(entid, td_nome);
            }
            if( $('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
                $('#'+tr_nome).css('display','');
                img.attr('src','../imagens/menos.gif');
            } else {
                $('#'+tr_nome).css('display','none');
                img.attr('src','/imagens/mais.gif');
            }
        }

        function editardirigente( entid, funid, opc, sit ){
            //#acao = ED - EDITAR DIRIGENTE (MEMBRO DO CONSELHO)
            //#acao = NV - CADASTRO DE UM NOVO DIRIGENTE
            if( typeof(opc) == 'undefined'){
               opc = 'NM'
            }
            var url = '?modulo=principal/editardirigente&acao=A&entid='+entid+'&funid='+funid+'&opc='+opc+'&sit='+sit;
            window.open(url, 'Dirigentes', 'scrollbars=yes,height=600,width=850,status=no,toolbar=no,menubar=no,location=no');
        }

        function excluirDirigente( entid, funid, mcsid ){
            if(confirm('Deseja excluir este registro')){
                $('#ex_entid').val(entid);
                $('#ex_funid').val(funid);
                $('#ex_mcsid').val(mcsid);
                $('#requisicao').val('excluirDirigente');
                $('#formulario').submit();
            }
        }

        //#CARREGAR LISTA FILHO
        function exibirListaDirigentes(entid, td_nome){
            divCarregando();
            $.ajax({
                type    : "POST",
                url     : window.location.href,
                data    : "requisicao=exibirListaDirigentes&entid="+entid,
                async: false,
                success: function(msg){
                    $('#'+td_nome).html(msg);
                    divCarregado();
                }
            });
        }

        function selecionarDirigenteCadastro( sit ){
            $.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=selecionarDirigenteCadastro&sit="+sit,
                success: function (resp){
                    $('#form_insert_cursos').html(resp);
                    $('#form_insert_cursos').modal();
                    divCarregado();
                }
            });
        }

    </script>

    <div id="form_insert_cursos" style="display: none;"></div>

<div class=''>
    <div class='col-md-2' style='position:static;'>
        <?=cria_aba_2($abacod_tela, $url, $parametros);?>
    </div>
    <div class='col-md-9' style='position:static;'>
        <? echo $autoriazacaoconcursos->cabecalho_entidade($entid);?>
    <form method="POST" id="formulario" name="formulario">
        <input type="hidden" id="requisicao" name="requisicao" value=""/>
        <input type="hidden" id="ex_entid" name="ex_entid" value=""/>
        <input type="hidden" id="ex_funid" name="ex_funid" value=""/>
        <input type="hidden" id="ex_mcsid" name="ex_mcsid" value=""/>

        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td colspan="6" class="subTituloCentro"> DIRIGENTES VIGENTES DA INSTITUIÇÃO </td>
            </tr>
            <tr>
                <td class="subtituloEsquerda" colspan="6">
                <?
                if(!$bloqueado):
                ?>
                <span onclick="selecionarDirigenteCadastro('A');"  style="cursor:pointer;">
                    <img src="../imagens/gif_inclui.gif" title="Cadastrar Dados">
                    Adicionar novo Dirigente
                </span>
                <?
                else:
                ?>
                <span>
                    <img src="../imagens/gif_inclui.gif" title="Operação Bloqueada!">
                    Adicionar novo Dirigente
                </span>
                <?
                endif;
                ?>
            </td>
        </tr>
            <tr>
                <td class="subTituloEsquerda"> Descrição </td>
                <td class="subTituloEsquerda"> Dirigentes </td>
                <td class="subTituloEsquerda"> Inicio da Vigência </td>
                <td class="subTituloEsquerda"> Fim da Vigência </td>
                <td class="subTituloEsquerda"> Situação </td>
                <td class="subTituloEsquerda"> Ato Legal </td>
            </tr>
<?PHP
            if( $dirigentes[0] ){
                foreach($dirigentes as $funent) {
                    if( $bloqueado && !$consulta){

                        if( $funent['entnome'] ){
                            $colunadirigente = "<img src='../imagens/alterar.gif' title='Operação Bloqueada!'> {$funent['entnome']}";
                        }else{
                            $colunadirigente = "<img src='../imagens/gif_inclui.gif' title='Operação Bloqueada!'> Não cadastrado";
                        }

                    } else {

                        if( $funent['entnome'] ){
                            $colunadirigente = "
                                <img src='../imagens/alterar.gif' onclick=\"editardirigente( {$funent['entid']}, {$funent['funid']}, 'ED', 'A' );\" title='Alterar Dados' style=\"cursor:pointer;\">";
                            
                            if($bloqueado){
                            $colunadirigente .= "
                                <img src='../imagens/excluir.gif' onclick=\"\" title='Excluir Dados' style=\"\"> {$funent['entnome']}";
                            }else{
                                $colunadirigente .= "
                                    <img src='../imagens/excluir.gif' onclick=\"excluirDirigente( {$funent['entid']}, {$funent['funid']}, '{$funent['mcsid']}' );\" title='Excluir Dados' style=\"cursor:pointer;\"> {$funent['entnome']}
                                ";
                            }
                        }else{
                            $colunadirigente = "<img src='../imagens/gif_inclui.gif' onclick=\"editardirigente( {$_SESSION['academico']['entid']}, {$funent['funid']}, 'NV', 'A' );\" title='Cadastrar Dados' style=\"cursor:pointer;\"> Não cadastrado";
                        }
                    }
                    
                    if( strtotime($funent['mcsvigenciafinal']) < strtotime( date('Y-m-d') ) ){
                        $style = 'style="color:red; font-size:12px;"';
                    }else{
                        $style = '';
                    }
                    
                    echo "<tr>";
                    echo "<td class='SubTituloDireita'> {$funent['fundsc']}: </td>";
                    echo "<td class='SubTituloCentro' style='text-align: left;'> {$colunadirigente} </td>";                    
                    echo "<td {$style} class=\"SubTituloCentro\">". formata_data( $funent['mcsvigenciainicial'] ) ."</td>";
                    echo "<td {$style} class=\"SubTituloCentro\">". formata_data( $funent['mcsvigenciafinal'] ) ."</td>";                    
                    echo "<td class='SubTituloCentro'> {$funent['mcssituacao']} </td>";
                    echo "<td class='SubTituloCentro'> {$funent['arqid']} </td>";
                    echo "</tr>";
                }
            }
?>
        </table>
    </form>

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td colspan="6" class="subTituloCentro"> DIRIGENTES ANTERIORES A VIGÊNCIA ATUAL</td>
        </tr>
        <tr>
            <td class="subtituloEsquerda">
                <?
                if(!$bloqueado):
                ?>
                <span onclick="selecionarDirigenteCadastro('I');"  style="cursor:pointer;">
                    <img src="../imagens/gif_inclui.gif" title="Cadastrar Dados">
                    Adicionar novo Dirigente
                </span>
                <?
                else:
                ?>
                <span>
                    <img src="../imagens/gif_inclui.gif" title="Operação Bloqueada!">
                    Adicionar novo Dirigente
                </span>
                <?
                endif;
                ?>
                
            </td>
        </tr>
        <tr>
            <td>
                <?PHP
                    liatgemDirigentesInstituicao( $v_entid );
                ?>
            </td>
        </tr>
    </table>

    <br>

    <table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>
        <tr>
            <td colspan="2" class="subTituloCentro"> <?=$titulo;?> </td>
        </tr>
        <tr>
            <td class="subtituloEsquerda">
                <?
                if(!$bloqueado):
                ?>
                <span onclick="editardirigente(<?=$entid;?>, '123', 'NV' );"  style="cursor:pointer;">
                    <img src="../imagens/gif_inclui.gif" title="Cadastrar Dados">
                    Adicionar novo Membro do Conselho
                </span>
                <?
                else:
                ?>
                <span>
                    <img src="../imagens/gif_inclui.gif" title="Operação Bloqueada!">
                    Adicionar novo Membro do Conselho
                </span>
                <?
                endif;
                ?>
            </td>
        </tr>
        <tr>
            <td>
                <br>
                <?PHP
                    listagemConselhoSuperior( $v_entid );
                ?>
            </td>
        </tr>

    </table>

    <br><br>
    <?PHP
        if( $_SESSION['academico']['entidadenivel'] != 'campus' ){
    ?>
            <table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>
                <tr>
                    <td colspan="2" class="subTituloCentro"> DIRIGENTES DOS CAMPUS E INTERLOCUTOR INSTITUCIONA ANTERIORES A VIGÊNCIA ATUAL </td>
                </tr>
                <tr>
                    <td>
                        <br>
                        <?PHP
                            $orgid = $_SESSION['academico']['orgid'];

                            listagemCampusEndidade( $v_entid, $orgid );
                        ?>
                    </td>
                </tr>
            </table>
    <?PHP
        }
    ?>
    </div>
</div>