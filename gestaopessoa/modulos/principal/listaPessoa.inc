<?PHP
    direcionaFT();

    if( $_POST['ajaxestuf'] ){
        header('content-type: text/html; charset=ISO-8859-1');
        $sql = "
            SELECT  muncod as codigo,
                    mundescricao as descricao
            FROM territorios.municipio

            WHERE estuf = '".$_POST['ajaxestuf']."'

            ORDER BY mundescricao asc
        ";
        echo $db->monta_combo( "filtro_muncod", $sql, 'S', 'Selecione...', '', '', '', '200', '', 'filtro_muncod' );
        die();
    }

    if( $_POST['ajaxmuncod'] ){
        header('content-type: text/html; charset=ISO-8859-1');
        $sql = "
            SELECT estuf
            FROM territorios.municipio
            WHERE muncod = '{$_POST['ajaxmuncod']}'
        ";
        die($db->pegaUm($sql));
    }

    if( $_POST['ajaxcpf'] != '' ){
        $_SESSION['fdpcpf'] = $_POST['ajaxcpf'];
        die();
    }

    if( $_POST['novaPessoa'] == 't' ){
        unset( $_SESSION['fdpcpf'] );
        die();
    }

    include  APPRAIZ."includes/cabecalho.inc";
    monta_titulo( 'Força de Trabalho', 'Lista de Pessoas' );
?>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/prototype.js"></script>
    
    <script>
        function validaForm(){
            document.formulario.submit();
        }
        
        function selecionarPessoa(fdpcpf){
            var req = new Ajax.Request(
                'gestaopessoa.php?modulo=principal/listaPessoa&acao=A', {
                method : 'post',
                parameters : '&ajaxcpf=' + fdpcpf,
                onComplete: function (res) {
                    window.location.href = '?modulo=principal/cadDadosPessoais&acao=A';
                }
            });
        }
        
        function cadastrarPessoa(){
            var req = new Ajax.Request(
                'gestaopessoa.php?modulo=principal/listaPessoa&acao=A', {
                method : 'post',
                parameters : '&novaPessoa=t',
                onComplete: function (res) {
                    window.location.href = '?modulo=principal/cadDadosPessoais&acao=A';
                }
            });
        }
        
        function filtraMunicipio(estuf){
            td_municipio = document.getElementById('td_municipio');
            td_estado	 = document.getElementById('td_estado');

            var req = new Ajax.Request(
                'gestaopessoa.php?modulo=principal/listaPessoa&acao=A', {
                method:     'post',
                parameters: '&ajaxestuf=' + estuf,
                onComplete: function (res){
                    td_municipio.innerHTML = res.responseText;
                }
            });

        }
        
        function filtraEstado(muncod){
            td_municipio = document.getElementById('td_municipio');
            td_estado	 = document.getElementById('td_estado');
            
            var estuf	 = document.getElementById('filtro_estuf');

            var req = new Ajax.Request(
                'gestaopessoa.php?modulo=principal/listaPessoa&acao=A', {
                method:     'post',
                parameters: '&ajaxmuncod=' + muncod,
                onComplete: function (res){
                    estuf.value = res.responseText;
                }
          });

        }
        
    </script>

    <form action="" method="POST" name="formulario">
        <input type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>"/>

        <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="tabela">
            <tr>
                <td class="SubTituloDireita" width="30%"> Nome da Pessoa: </td>
                <td>
                    <?PHP
                        echo campo_texto('filtro_fdpnome', 'N', 'S', '', 60, 200, '', '');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita"> CPF: </td>
                <td>
                    <?PHP
                        echo campo_texto('filtro_fdpcpf', 'N', 'S', '', 20, 14, '###.###.###-##', '', '', '', '', '', 'this.value=mascaraglobal(\'###.###.###-##\',this.value);');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita"> SIAPE: </td>
                <td>
                    <?PHP
                        echo campo_texto('filtro_estuf', 'N', 'S', '', 20, 7, '#######', '');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita"> Lotação: </td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  fulid AS codigo,
                                    fuldescricao AS descricao
                            FROM gestaopessoa.ftunidadelotacao
                        ";
                        $db->monta_combo("filtro_fulid", $sql, 'S', 'Selecione...', '', '', '', 400);
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita"> Vínculo com o MEC: </td>
                <td>
                    <?PHP
                        $perfil = arrayPerfil();
                        $sql = "
                            SELECT  fstid AS codigo,
                                    fstdescricao AS descricao
                            FROM gestaopessoa.ftsituacaotrabalhador
                        ";
                        if( in_array(PERFIL_SERVIDOR, $perfil) || in_array(PERFIL_TERCEIRIZADO, $perfil) || in_array(PERFIL_CONSULTOR, $perfil) ) {
                            $sql.= "WHERE fstid in ( " . implode(",", controlaPefilFT('vinculosPermitidos')) . ")";
                        } else {
                            $sql.= "WHERE fstid <> 4";
                        }
                        $db->monta_combo('filtro_fstid', $sql, 'S', "Selecione...", '', '', '', '200', 'N', 'filtro_fstid');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita"> Estado: </td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  estuf  AS codigo,
                                    estdescricao AS descricao
                            FROM territorios.estado
                        ";
                        $db->monta_combo('filtro_estuf', $sql, 'S', "Selecione...", 'filtraMunicipio(this.value)', '', '', '200', 'N', 'filtro_estuf');
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
                        $db->monta_combo('filtro_muncod', $sql, 'S', "Selecione...", '', '', '', '200', 'N', 'filtro_muncod');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita"> Grau de Escolaridade: </td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  tgeid as codigo,
                                    tgedescricao as descricao
                            FROM public.tipograuescolaridade
                        ";
                        $db->monta_combo('filtro_tgeid', $sql, 'S', "Selecione...", '', '', '', '200', 'N', 'filtro_tgeid');
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
                        $db->monta_combo("filtro_fdpsexo", $sql, 'S', 'Selecione...', '', '', '', '100', '', 'filtro_fdpsexo');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita"> Idade: </td>
                <td>
                    <?PHP
                        echo campo_texto('filtro_idade', 'N', 'S', 'Idade', 10, 2, '##', '', '', '', 0, 'id="filtro_idade"', '', $filtro_idade, '', null);
                    ?>
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
                    <img align="absmiddle" src="/imagens/gif_inclui.gif"/> Cadastrar Pessoa
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
    if( $_REQUEST['filtro_estuf']){
        $and .= "AND fdp.estuf = '".$_REQUEST['filtro_estuf']."' ";
    }
    if( $_REQUEST['filtro_muncod']){
        $and .= "AND fdp.muncod = '".$_REQUEST['filtro_muncod']."' ";
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
    if( in_array(  PERFIL_SERVIDOR , $perfil ) || in_array(  PERFIL_TERCEIRIZADO, $perfil ) || in_array(  PERFIL_CONSULTOR , $perfil ) ){
        $and_= "AND fdp.fstid in ( ".implode(",",controlaPefilFT('vinculosPermitidos') ).")";
    }

    //filtro para perfil Gestor e AIDP
    verificaPerfilGestoreAidp();
    //fim filtro

    $acao = " <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"javascript: selecionarPessoa(\''||fdp.fdpcpf||'\');\" title=\"Selecionar Pessoa\"> ";

    $sql = "
        SELECT  DISTINCT '{$acao}',
                fdp.fdpcpf,
                UPPER( fdp.fdpnome ) AS fdpnome,
                fdp.fdpsiape,
                fst.fstdescricao,
                ftu.fuldescricao,
                '-- ' as fdpalteradopor
        FROM gestaopessoa.ftdadopessoal as fdp

        JOIN gestaopessoa.ftsituacaotrabalhador AS fst ON fst.fstid = fdp.fstid
        LEFT JOIN gestaopessoa.ftdadofuncional AS fdt ON fdt.fdpcpf = fdp.fdpcpf
        LEFT JOIN gestaopessoa.ftunidadelotacao as ftu ON ftu.fulid = fdt.fulid
        LEFT JOIN gestaopessoa.ftformacaoacademica as formacao ON fdp.fdpcpf = formacao.fdpcpf
        LEFT JOIN gestaopessoa.idioma as idioma ON fdp.fdpcpf = idioma.fdpcpf
        LEFT JOIN gestaopessoa.ftatividadedesenvolvida as atividadedesenv ON fdp.fdpcpf = atividadedesenv.fdpcpf
        LEFT JOIN gestaopessoa.ftexperienciaanterior AS fte ON fdp.fdpcpf = fte.fdpcpf

        WHERE fdp.fdpcpf IS NOT NULL AND fst.fstid <> 4
		".($filtroGestor ? " AND fst.fstid in (".$filtroGestor.") " : '' )."
		".($filtroAidp ? " AND fdt.fulid in (".$filtroAidp.") " : '' )."
		$and_
		$and

        ORDER BY fdpnome
    ";
    $cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;Ação", "CPF","NOME", "SIAPE","Vínculo com o MEC","Lotação","Alterado por" );
    $db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', ''); 
?>