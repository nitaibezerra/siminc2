<?PHP

    //Filtra municípios
    if ($_REQUEST['filtraMunicipio'] && $_REQUEST['estuf']) {
        //header('content-type: text/html; charset=ISO-8859-1');
        filtraMunicipio($_REQUEST['estuf']);
        exit;
    }

    //pega perfil do usuario
    $pflcod = pegaPerfil($_SESSION['usucpf']);


    include APPRAIZ . 'includes/cabecalho.inc';
    echo '<br />';
    $titulo = "PSE - Programa Saúde na Escola";
    monta_titulo($titulo, '');

    if($pflcod == SECRETARIA_ESTADUAL || $pflcod == SECRETARIA_MUNICIPAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == ESCOLA_ESTADUAL || $pflcod == DIRETOR_ESCOLA || $pflcod == EDUCADOR_ESCOLA || $pflcod == MEC || $pflcod == SUPER_USUARIO || $pflcod == CONSULTA) {
?>
    <!--
    <table width="95%" align="center" cellspacing="1" cellpadding="3">
        <tr>
            <td align="right">
                <div style="float:center;position:relative;width:300px;">
                    <fieldset style="background-color:#ffffff;">
                        <legend>Manual de Instruções</legend>
                            <table width="100%" >
                                <tr>
                                    <td valign="bottom" height="50%" style="padding-right:15px;">
<?PHP
        if ($pflcod == SECRETARIA_ESTADUAL || $pflcod == SECRETARIA_MUNICIPAL) {
            echo '<a href="/pse/arquivos/instrutivo_liberar_senha.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Liberar Senha</a>';
            echo '<br><a href="/pse/arquivos/instrutivo_saude.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Monitoramento de Ações</a>';
        } elseif ($pflcod == ESCOLA_MUNICIPAL || $pflcod == ESCOLA_ESTADUAL) {
            echo '<a href="/pse/arquivos/instrutivo_saude.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Monitoramento de Ações</a>';
        } elseif ($pflcod == DIRETOR_ESCOLA) {
            echo '<a href="/pse/arquivos/instrutivo_diretor.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Monitoramento de Ações</a>';
        } elseif ($pflcod == EDUCADOR_ESCOLA) {
            echo '<a href="/pse/arquivos/instrutivo_educador.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Monitoramento de Ações</a>';
        } elseif ($pflcod == MEC || $pflcod == SUPER_USUARIO || $pflcod == CONSULTA) {
            echo '<a href="/pse/arquivos/instrutivo_liberar_senha.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Liberar Senha</a>';
            echo '<br><a href="/pse/arquivos/instrutivo_saude.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Perfil Saúde</a>';
            echo '<br><a href="/pse/arquivos/instrutivo_diretor.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Perfil Diretor</a>';
            echo '<br><a href="/pse/arquivos/instrutivo_educador.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Perfil Educador</a>';
        }
        echo '<br><a href="/pse/arquivos/instrutivo_financeiro.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Incentivo Financeiro (Anexo I)</a>';
        echo '<br><a href="/pse/arquivos/instrutivo_financeiro2.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Incentivo Financeiro (Anexo II)</a>';
        echo '<br><a href="/pse/arquivos/instrutivo_financeiro3.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Incentivo Financeiro (Anexo III)</a>';
?>
                                </td>
                            </tr>
                        </table>
                    </fieldset>
                </div>
            </td>
        </tr>
    </table>
    -->
<?PHP

    }

    echo '<br>';

    //$db->cria_aba( $abacod_tela, $url, '' );

    $menu = carregarMenuAbasPse();
    echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

    monta_titulo('Lista de Escolas - Nutrisus', 'Filtro de Pesquisa');

    $where = "";

    if ($_POST) {
        if (!empty($_REQUEST['entid'])) {

            $_SESSION['pse']['entid'] = $_REQUEST['entid'];
            $_SESSION['pse']['sse'] = "N";

            $sql = "SELECT muncod FROM entidade.endereco WHERE entid = '{$_SESSION['pse']['entid']}'";
            $_SESSION['pse']['muncod'] = $db->pegaUm($sql);

            echo "<script>window.location.href='pse.php?modulo=principal/nutrisus&acao=A';</script>";
            exit;
        }
        $where = ($_POST["entnome"]) ? " AND UPPER(ent.entnome) like UPPER('%" . $_POST["entnome"] . "%')" : "";
        $where.=($_POST["estuf"]) ? " AND ende.estuf = '" . $_POST["estuf"] . "'" : "";
        $where.=($_POST["muncod"]) ? " AND ende.muncod = '" . $_POST["muncod"] . "'" : "";
        $where.=($_POST["entcodent"]) ? " AND ent.entcodent = '" . $_POST["entcodent"] . "'" : "";
    }

    //cria sessoes para o pse (nao retirar a linha abaixo)
    include APPRAIZ . 'pse/modulos/principal/permissao.inc';

    
    /**
     * Exclusão do poupup que avisa sobre o prazo adiado(30/04/2014) no dia 29/04/2014
     * Obs: Solicitado pelo Juvenal.
     */

?>
<form id="formPesquisaEscola" name="formPesquisaEscola" method="post" action="">
    <input type="hidden" name="entid" id="entid">

<?PHP

    if ($pflcod != ESCOLA_MUNICIPAL && $pflcod != ESCOLA_ESTADUAL && $pflcod != DIRETOR_ESCOLA && $pflcod != EDUCADOR_ESCOLA) {

?>
        <table id="pesquisa" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
            <tr>
                <td width="35%" class="SubTituloDireita" valign="top"><b>INEP:</b></td>
                <td>
                    <?PHP
                        $entcodent = $_REQUEST["entcodent"];
                        echo campo_texto('entcodent', 'N', 'S', 'Código INEP', 50, 500, '', '','');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" valign="top"><b>Nome da Escola:</b></td>
                <td>
                    <?PHP
                        $entnome = $_REQUEST["entnome"];
                        echo campo_texto('entnome', 'N', 'S', 'Nome da Escola', 50, 500, '', '');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" valign="top"><b>UF:</b></td>
                <td>
                    <?PHP
                        $estuf = ($uf == '' ? $_REQUEST['estuf'] : $uf);

                        $sql = "
                            SELECT  estuf AS codigo,
                                    estdescricao AS descricao
                            FROM territorios.estado
                            ORDER BY estdescricao
                        ";
                        $db->monta_combo("estuf", $sql, $comboUFhab, 'Selecione...', 'filtraMunicipio', '', '', '215', '', '', '', $estuf);
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" valign="top"><b>Município:</b></td>
                <td id="td_municipio">
                    <?
                        $muncod = ($muncod == '' ? $_REQUEST['muncod'] : $muncod);
                        $sql = "
                            SELECT  ter.muncod AS codigo,
                                    ter.mundescricao AS descricao
                                    
                            FROM territorios.municipio ter
                            
                            WHERE ter.estuf = '{$estuf}'
                            ORDER BY ter.estuf, ter.mundescricao
                        ";
                        $db->monta_combo("muncod", $sql, $comboMUNhab, 'Selecione...', '', '', '', '215', 'N', '', '', '', $muncod);
                    ?>
                </td>
            </tr>
            <tr>
                <td bgcolor="#c0c0c0"></td>
                <td align="left" bgcolor="#c0c0c0">
                    <input type="button" id="bt_pesquisar" <?= $btHab; ?> value="Pesquisar" onclick="pesquisar()" />
                </td>
            </tr>
        </table>

        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
            <tr>
                <td align="center" colspan="2"><b>Lista de Escolas</b></td>
            </tr>
        </table>
<?PHP
    }
?>
</form>
    
<?PHP
    $sql = "
        SELECT  DISTINCT '<a href=\"#\" onclick=\"EditarCadastro(\'' || ent.entid || '\');\" title=\"Editar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a> ' as acao,
                ent.entnome, 
                m.estuf, 
                m.mundescricao

        FROM entidade.entidade ent

        INNER JOIN pse.escolabasepse esc ON ent.entcodent = esc.idecodinep
        INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
        INNER JOIN entidade.funcaoentidade fe ON fe.entid = ent.entid
        INNER JOIN territorios.municipio m ON m.muncod = ende.muncod

        WHERE esc.ideanocenso = ".ANO_CENSO." AND ent.entstatus='A' {$where}

        ORDER BY m.estuf,m.mundescricao,ent.entnome
    ";
    $cabecalho = array("Ação", "Escola", "UF", "Município");
    $tamanho = array('10%', '50%', '10%', '30%');
    $alinhamento = array('center', 'left', 'center', 'center');

    if ($pflcod == MEC || $pflcod == SUPER_USUARIO) {
        if ($_POST) {
            $db->monta_lista($sql, $cabecalho, 25, 10, 'N', 'center', '', '', $tamanho, $alinhamento);
        }
    } else {
        $db->monta_lista($sql, $cabecalho, 25, 10, 'N', 'center', '', '', $tamanho, $alinhamento);
    }

   
?>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">

    function filtraMunicipio(estuf) {
        if (estuf != '') {
            var destino = document.getElementById("td_municipio");
            var myAjax = new Ajax.Request(
                window.location.href,
                {
                    method: 'post',
                    parameters: "filtraMunicipio=true&" + "estuf=" + estuf,
                    asynchronous: false,
                    onComplete: function(resp) {
                        if (destino) {
                            destino.innerHTML = resp.responseText;
                        }
                    },
                    onLoading: function() {
                        destino.innerHTML = 'Carregando...';
                    }
                });
        }
    }
    
    var btPesquisa = document.getElementById("bt_pesquisar");

    function pesquisar() {
        <? if ($pflcod == MEC || $pflcod == SUPER_USUARIO) { ?>
                if (document.formPesquisaEscola.entcodent.value == '' && document.formPesquisaEscola.entnome.value == '' && document.formPesquisaEscola.estuf.value == '' && document.formPesquisaEscola.muncod.value == ''){
                    alert("Preencha pelo menos um item para pesquisar.");
                    return false;
                }
        <? } ?>

        btPesquisa.disabled = true;
        document.formPesquisaEscola.submit();
    }

    function EditarCadastro(entid) {
        document.getElementById("entid").value = entid;
        document.formPesquisaEscola.submit();
    }

</script>