<?PHP
    //direcionaFT();

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    function comboFiltroMunicipio( $dados ){
        global $db;
        $uf = $dados['uf'];

        if( $uf != '' ){
            header('content-type: text/html; charset=ISO-8859-1');
            $sql = "
                SELECT  muncod as codigo,
                        mundescricao as descricao
                FROM territorios.municipio

                WHERE estuf = '{$uf}'

                ORDER BY mundescricao asc
            ";
            $db->monta_combo('filtro_muncod', $sql, 'S', "Selecione...", '', '', '', '220', 'N', 'filtro_muncod', false, $filtro_muncod, null);
            die();
        }
    }    

    include  APPRAIZ."includes/cabecalho.inc";
    monta_titulo( 'Força de Trabalho', 'Lista de Pessoas' );

    $abacod_tela    = ABA_FORCA_TRAB_MENU_INICIAL;
    $url            = 'gestaopessoa.php?modulo=principal/forca_trabalho/lista_grid_servidores&acao=A';
    $parametros     = '';

    $db->cria_aba($abacod_tela, $url, $parametros);
?>

    <link type="text/css" rel="stylesheet" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
    <script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

    <script type="text/javascript">
        function atribuiCPFSessao(fdpcpf){
            $.ajax({
                type    : "POST",
                url     : window.location,
                data    : "requisicao=atribuiCPFSessao&fdpcpf="+fdpcpf,
                asynchronous: false,
                success: function( msg ){
                    var resp = pegaRetornoAjax('<resp>', '</resp>', msg, true);
                    if( trim(resp) == 'OK' ){
                        window.location.href = '?modulo=principal/forca_trabalho/cad_dados_pessoais&acao=A&opc=FT';
                    }
                }
            });
        }

        function cadastrarPessoa(){
            $.ajax({
                type    : "POST",
                url     : window.location,
                data    : "requisicao=cadastrarPessoa",
                asynchronous: false,
                success: function( msg ){
                    var resp = pegaRetornoAjax('<resp>', '</resp>', msg, true);
                    if( trim(resp) == 'OK' ){
                        window.location.href = '?modulo=principal/forca_trabalho/cad_dados_pessoais&acao=A&acao_ef=NV&opc=FT';
                    }
                }
            });
        }

        function comboFiltroMunicipio( uf ){
            $.ajax({
                type    : "POST",
                url     : window.location,
                data    : "requisicao=comboFiltroMunicipio&uf="+uf,
                asynchronous: false,
                success: function( resp ){
                    $('#td_municipio').html( resp );
                }
            });
        }

        function validaForm(){
            document.formulario.submit();
        }
    </script>

    <form action="" method="POST" name="formulario">
        <input type="hidden" id="requisicao" name="requisicao" value=""/>
        <input type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>"/>

        <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="tabela">
            <tr>
                <td class="SubTituloDireita" width="30%"> Nome da Pessoa: </td>
                <td>
                    <?PHP
                        echo campo_texto('filtro_fdpnome', 'N', 'S', '', 75, 200, '', '');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita"> CPF: </td>
                <td>
                    <?PHP
                        echo campo_texto('filtro_fdpcpf', 'N', 'S', '', 30, 14, '###.###.###-##', '', '', '', '', '', 'this.value=mascaraglobal(\'###.###.###-##\',this.value);');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita"> SIAPE: </td>
                <td>
                    <?PHP
                        echo campo_texto('filtro_estuf', 'N', 'S', '', 30, 7, '#######', '');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita"> Sexo: </td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  'm' as codigo,
                                    'Masculino' as descricao
                            UNION
                            SELECT  'f' AS codigo,
                                    'Feminino' AS descricao
                        ";
                        $db->monta_combo("filtro_fdpsexo", $sql, 'S', 'Selecione...', '', '', '', '220', 'N', 'filtro_fdpsexo', false, $filtro_fdpsexo, null);
                    ?>
                </td>
            </tr>            
<!--            <tr>
                <td class="SubTituloDireita"> Idade: </td>
                <td>
                    <?PHP
                        echo campo_texto('filtro_idade', 'N', 'S', 'Idade', 10, 2, '##', '', '', '', 0, 'id="filtro_idade"', '', $filtro_idade, '', null);
                    ?>
                </td>
            </tr>-->
<!--            <tr>
                <td class="SubTituloDireita"> Lotação: </td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  fulid AS codigo,
                                    fuldescricao AS descricao
                            FROM gestaopessoa.ftunidadelotacao
                        ";
                        $db->monta_combo("filtro_fulid", $sql, 'S', 'Selecione...', '', '', '', '480', 'N', 'filtro_fulid', false, $filtro_fulid, null);
                    ?>
                </td>
            </tr>-->
            <tr>
                <td class="SubTituloDireita"> Vínculo com o MEC: </td>
                <td>
                    <?PHP
                        $perfil = arrayPerfil();
                        $sql = "
                            SELECT  fstid AS codigo,
                                    fstdescricao AS descricao
                            FROM gestaopessoa.ftsituacaotrabalhador
                            WHERE fstid <> 4;
                        ";
                        $db->monta_combo('filtro_fstid', $sql, 'S', "Selecione...", '', '', '', '220', 'N', 'filtro_fstid', false, $filtro_fstid, null);
                    ?>
                </td>
            </tr>
<!--            <tr>
                <td class="SubTituloDireita"> Estado: </td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  estuf  AS codigo,
                                    estdescricao AS descricao
                            FROM territorios.estado
                        ";
                        $db->monta_combo('filtro_estuf', $sql, 'S', "Selecione...", 'comboFiltroMunicipio(this.value)', '', '', '220', 'N', 'filtro_estuf', false, $filtro_estuf, null);
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita"> Município: </td>
                <td id="td_municipio">
                    <?PHP
                        $sql = "
                            SELECT  muncod AS codigo,
                                    mundescricao AS descricao
                            FROM territorios.municipio
                        ";
                        $db->monta_combo('filtro_muncod', $sql, 'S', "Selecione...", '', '', '', '220', 'N', 'filtro_muncod', false, $filtro_muncod, null);
                    ?>
                </td>
            </tr>-->
            <tr>
                <td class="SubTituloDireita"> Grau de Escolaridade: </td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  tgeid as codigo,
                                    tgedescricao as descricao
                            FROM public.tipograuescolaridade
                        ";
                        $db->monta_combo('filtro_tgeid', $sql, 'S', "Selecione...", '', '', '', '220', 'N', 'filtro_tgeid', false, $filtro_tgeid, null);
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita">Gratificação:</td>
                <td>
                    <input type="radio" name="ftptipogratificacao" id="ftptipogratificacao_TD" value="TD" checked="checked"> <b>Todas</b>
                    <input type="radio" name="ftptipogratificacao" id="ftptipogratificacao_PE" value="PE"> <b>GDPGPE</b>
                    <input type="radio" name="ftptipogratificacao" id="ftptipogratificacao_CE" value="CE"> <b>GDACE</b>
                    <input type="radio" name="ftptipogratificacao" id="ftptipogratificacao_PS" value="PS"> <b>GDAPS</b>
                    <input type="radio" name="ftptipogratificacao" id="ftptipogratificacao_OS" value="OS"> <b>CEDIDO</b>
                </td>
            </tr>
            <tr>
                <td class="SubTituloCentro" colspan="2">
                    <input type="button" name="" value="Pesquisar" onclick="return validaForm();"/>
                </td>
            </tr>
        </table>
    </form>

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="tabela">
        <tr>
            <td style="padding: 10px;">
                <span style="cursor: pointer" onclick="cadastrarPessoa();" title="Nova">
                    <img align="absmiddle" src="/imagens/gif_inclui.gif"/> Cadastrar Servidor (pessoa)
                </span>
            </td>
        </tr>
    </table>

<?PHP
    $and = " ";

    if( $_REQUEST['filtro_ftu_fulid']){
        $and .= "AND ftu.fulid = ".$_REQUEST['filtro_ftu_fulid']." ";
    }
    if( $_REQUEST['filtro_fst_fstid']){
        $and .= "AND fst.fstid = ".$_REQUEST['filtro_fst_fstid']." ";
    }
    if( $_REQUEST['filtro_tfo_tfoid']){
        $and .= "AND formacao.tfoid = ".$_REQUEST['filtro_tfo_tfoid']." ";
    }
    if( $_REQUEST['filtro_fti_ftiid']){
        $and .= "AND idioma.ftiid = ".$_REQUEST['filtro_fti_ftiid']." ";
    }
    if( $_REQUEST['filtro_fta_ftaid']){
        $and .= "AND atividadedesenv.ftaid = ".$_REQUEST['filtro_fta_ftaid']." ";
    }
    if( $_REQUEST['filtro_fta_fnaid']){
        $and .= "AND atividadedesenv.fnaid = ".$_REQUEST['filtro_fta_fnaid']." ";
    }
    if( $_REQUEST['filtro_fte_fteid']){
        $and .= "AND fte.fteid = ".$_REQUEST['filtro_fte_fteid']." ";
    }
    if( $_REQUEST['filtro_fte_fneid']){
        $and .= "AND fte.fneid = ".$_REQUEST['filtro_fte_fneid']." ";
    }
    if( $_REQUEST['filtro_fdpnome']){
        $and .= "AND fdp.fdpnome ILIKE '%".$_REQUEST['filtro_fdpnome']."%' ";
    }
    if( $_REQUEST['filtro_fdpcpf']){
        $and .= "AND fdp.fdpcpf = '".str_replace( ".", "",str_replace("-","", $_REQUEST['filtro_fdpcpf'] ) )."' ";
    }
    if( $_REQUEST['filtro_fdpsexo']){
        $and .= "AND fdp.fdpsexo = '".$_REQUEST['filtro_fdpsexo']."' ";
    }
    if( $_REQUEST['filtro_tgeid']){
        $and .= "AND ".$_REQUEST['filtro_tgeid']." IN ( SELECT tfoid FROM gestaopessoa.ftformacaoacademica WHERE fdpcpf = fdp.fdpcpf ) ";
    }
    if( $_REQUEST['filtro_idade']){
        $and .= "AND (( ".date("Y")." - to_number( to_char( fdp.fdpdatanascimento, 'yyyy') , '9999') ) = ".$_REQUEST['filtro_idade']." ) ";
    }
    if( $_REQUEST['filtro_fstid']){
        $and .= "AND fdp.fstid = ".$_REQUEST['filtro_fstid']." ";
    }
    
    if( $_REQUEST['ftptipogratificacao'] != 'TD' ){
        switch( $_REQUEST['ftptipogratificacao'] ){
            case 'PE':
                $and .= "AND fdp.ftptipogratificacao = 'PE'";
            break;
            case 'CE':
                $and .= "AND fdp.ftptipogratificacao = 'CE'";
            break;
            case 'PS':
                $and .= "AND fdp.ftptipogratificacao = 'PS'";
            break;
            case 'OS':
                $and .= "AND fdp.ftptipogratificacao = 'OS'";
            break;
        }
    }
    
    #FILTRO PARA PERFIL GESTOR E AIDP
    verificaPerfilGestoreAidp();
    
    $acao = " <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"atribuiCPFSessao(\''||fdp.fdpcpf||'\');\" title=\"Selecionar Pessoa\"> ";

    $sql = "
        SELECT  DISTINCT '{$acao}',
                fdp.fdpcpf,
                UPPER( fdp.fdpnome ) AS fdpnome,
                fdp.fdpsiape,
                fst.fstdescricao,
                ftu.fuldescricao,
                tg.tgedescricao,
                
                CASE 
                    WHEN fdp.ftptipogratificacao = 'PE' THEN 'GDPGPE'
                    WHEN fdp.ftptipogratificacao = 'CE' THEN 'GDACE'
                    WHEN fdp.ftptipogratificacao = 'PS' THEN 'GDAPS'
                    WHEN fdp.ftptipogratificacao = 'OS' THEN 'CEDIDO'
                END as ftptipogratificacao
                
        FROM gestaopessoa.ftdadopessoal as fdp

        JOIN gestaopessoa.ftsituacaotrabalhador AS fst ON fst.fstid = fdp.fstid
        
        LEFT JOIN gestaopessoa.ftdadofuncional AS fdt ON fdt.fdpcpf = fdp.fdpcpf
        LEFT JOIN gestaopessoa.ftunidadelotacao AS ftu ON ftu.fulid = fdt.fulid
        LEFT JOIN gestaopessoa.ftformacaoacademica AS formacao ON fdp.fdpcpf = formacao.fdpcpf
        LEFT JOIN public.tipograuescolaridade AS tg ON tg.tgeid = formacao.tfoid
        LEFT JOIN gestaopessoa.idioma AS idioma ON fdp.fdpcpf = idioma.fdpcpf
        LEFT JOIN gestaopessoa.ftatividadedesenvolvida AS atividadedesenv ON fdp.fdpcpf = atividadedesenv.fdpcpf
        LEFT JOIN gestaopessoa.ftexperienciaanterior AS fte ON fdp.fdpcpf = fte.fdpcpf

        WHERE fdp.fdpcpf IS NOT NULL AND fst.fstid <> 4 ".($filtroGestor ? " AND fst.fstid in (".$filtroGestor.") " : '' )." ".($filtroAidp ? " AND fdt.fulid in (".$filtroAidp.") " : '' )."
	
        {$and_} {$and}

        ORDER BY fdpnome
    ";
    $cabecalho = array("Ação", "CPF", "Nome", "SIAPE", "Vínculo com o MEC", "Lotação", "Formação", "Gratificação" );
    $alinhamento = Array('center', '', '', '', '', '', '');
    $tamanho = Array('3%', '', '', '', '', '', '');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
?>