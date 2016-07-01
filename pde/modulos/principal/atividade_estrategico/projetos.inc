<?PHP
    if (!$_REQUEST['completo']) {
        header("Location: estrategico.php?modulo=principal/alinhamento_estrategico&acao=A");
        die;
    }
    $arrPerfil = pegaPerfilGeral();

    if ( !( in_array(PERFIL_SUPER_USUARIO, $arrPerfil) || in_array(PERFIL_ADMINISTRADOR, $arrPerfil)) ){
        
        if ((in_array(PERFIL_EXECUTOR, $arrPerfil) || in_array(PERFIL_VALIDADOR, $arrPerfil)) && !$_SESSION['estrategico']['visitou_pendencias']) {
            $_SESSION['estrategico']['visitou_pendencias'] = true;
            header("Location: estrategico.php?modulo=principal/atividade_estrategico/minhasPendencias&acao=A");
        }

        if ( in_array(PERFIL_CONSULTA_EXTERNA, $arrPerfil) || in_array(PERFIL_CONSULTA_COCKPIT, $arrPerfil) ){
            header("Location: estrategico.php?modulo=principal/atividade_estrategico/minhasPendencias&acao=A");
        }
    }

    if (in_array(PERFIL_SUPER_USUARIO, $arrPerfil) || in_array(PERFIL_ADMINISTRADOR, $arrPerfil)) {//Não possui filtros

    } elseif (in_array(CONSULTA_GERAL, $arrPerfil)) {//Não possui filtros e não pode editar as informações
        $boNaoPodeEditar = true;
    } elseif (in_array(591, $arrPerfil) || in_array(732, $arrPerfil)) {//Filtra apenas seus projetos
        $arrWhere[] = "a.atiid in (select distinct ure.atiid from pde.usuarioresponsabilidade ure where usucpf = '{$_SESSION['usucpf']}' and rpustatus ='A' and pflcod in(591,732))";
    } elseif (in_array(PERFIL_EXECUTOR, $arrPerfil) || in_array(PERFIL_VALIDADOR, $arrPerfil)) {//Filtra apenas seus projetos e não pode editar as informações
        $boNaoPodeEditar = true;
        $arrWhere[] = "
            a.atiid in (select
                            distinct _atiprojeto
                    from
                            pde.monitoraetapascontrole eta
                    inner join
                            pde.monitorametaentidade me ON me.mnmid = eta.mnmid and eta.tpvid = me.tpvid
                    inner join
                            entidade.entidade ent ON ent.entid = me.entid
                    inner join
                            pde.monitorameta mnm ON mnm.mnmid = eta.mnmid
                    inner join
                            pde.monitoraitemchecklist mic ON mic.micid = mnm.micid
                    inner join
                            pde.atividade ati ON ati.atiid = mic.atiid
                    where
                            ent.entnumcpfcnpj = '{$_SESSION['usucpf']}'
                    and
                            eta.tpvid in(1,2)
              )
        ";
    }

    if ($_POST) {
        extract($_POST);
    }
    // VERIFICA DE USUÁRIO POSSUI PERFIL PARA ALOCAÇÃO DE DALAS
    $usuario_alocacao_salas = arrayPerfil();

    if (in_array(PERFIL_ALOCACAO_SALAS, $usuario_alocacao_salas) && count($usuario_alocacao_salas) == 1) {
        redirecionar('principal/atividade_estrategico/listaEnem', 'A');
    } elseif (in_array(PERFIL_ALOCACAO_SALAS, $usuario_alocacao_salas) && count($usuario_alocacao_salas) > 1) {
        $atiidENEM = "55168";
    }

    // VERIFICA SE USUÁRIO É GERENTE
    $usuario_gerente_projeto = usuario_possui_perfil(PERFIL_GESTOR) || $db->testa_superuser();

    // SELECIONA PROJETO
    if ($_REQUEST['selecionar_projeto']) {
        # TODO: verificar se o projeto existe
        $_SESSION['projeto'] = (integer) $_REQUEST['selecionar_projeto'];
        redirecionar('principal/atividade_estrategico/arvore', 'A');
    } else {
        $_SESSION['projeto'] = null;
    }

    // CADASTRAR PROJETO
    if ($_REQUEST['cadastrar_projeto'] && $usuario_gerente_projeto) {
        $descricao = $_REQUEST['cadastrar_projeto'];
        if (!empty($descricao)) {
            $atiid = $db->pegaUm("select nextval( 'pde.atividade_atiid_seq' )");
            $sql = sprintf(
                "insert into pde.atividade ( atiid, _atiprojeto, atidescricao, usucpf, atiordem, atiestrategico, atiexercicio ) values ( %d, %d, '%s', '%s', 0, true, " . date('Y') . " )", $atiid, $atiid, $descricao, $_SESSION['usucpf']
            );
            if ($db->executar($sql)) {
                atividade_atribuir_responsavel($atiid, PERFIL_GESTOR, array($_SESSION['usucpf']));
                $db->commit();
            }
        }
        redirecionar($_REQUEST['modulo'], $_REQUEST['acao']);
    }

    // REMOVER
    if ($_REQUEST['remover'] && $usuario_gerente_projeto) {
        $atiid_remover = (integer) $_REQUEST['remover'];
        $sql = "select count(*) from pde.atividade where atiidpai = " . $atiid_remover . " and atistatus = 'A'";
        if (!$db->pegaUm($sql)) {
            $sql = "update pde.atividade set atistatus = 'I' where atiid = " . $atiid_remover;
            $db->executar($sql);
            $db->commit();
        }
        redirecionar($_REQUEST['modulo'], $_REQUEST['acao']);
    }

    // CAPTURA PROJETOS DO USUÁRIO
    $where = "";
    #if ( !$db->testa_superuser() || !usuario_possui_perfil( PERFIL_CONSULTA, $_SESSION['usucpf'] ) ) {
    $consulta = usuario_possui_perfil(PERFIL_ADMINISTRADOR, $_SESSION['usucpf']) || usuario_possui_perfil(PERFIL_CONSULTA, $_SESSION['usucpf']);
    if (!( $db->testa_superuser() || $consulta )) {
    //	$sql = "
    //		select
    //			proj.atiid
    //		from pde.usuarioresponsabilidade ur
    //			inner join pde.atividade a on
    //				a.atiid = ur.atiid
    //			inner join pde.atividade proj on
    //				proj.atiid = a._atiprojeto
    //			inner join seguranca.perfilusuario pu on
    //				pu.pflcod = ur.pflcod
    //		where
    //			ur.usucpf = '" . $_SESSION['usucpf'] . "' and
    //			a.atistatus = 'A' and
    //			proj.atistatus = 'A'
    //		group by
    //			proj.atiid
    //	";
        if ($atiidENEM) {
            $sqlUnion = "
                union
                select a._atiprojeto
                from pde.atividade a
                inner join pde.usuarioresponsabilidade ur on
                        ur.atiid = a.atiid
                inner join seguranca.perfilusuario pu on
                        pu.pflcod = ur.pflcod
                where a.atistatus = 'A' and a.atiid = '$atiidENEM'
                group by a._atiprojeto
            ";
        }

        $sql = "
            select a._atiprojeto
            from pde.atividade a
            inner join pde.usuarioresponsabilidade ur on
                    ur.atiid = a.atiid
            inner join seguranca.perfilusuario pu on
                    pu.pflcod = ur.pflcod
            where a.atiestrategico = true and a.atistatus = 'A' --and ur.usucpf = '" . $_SESSION['usucpf'] . "'
            group by a._atiprojeto
            $sqlUnion
        ";

        $projetos = $db->carregar($sql);
        $projetos = $projetos ? $projetos : array();
        $where = array(0);
        if ($projetos) {
            foreach ($projetos as $item) {
                array_push($where, $item['_atiprojeto']);
            }
        }
        $where = " a.atiid in ( " . implode(',', $where) . " ) and ";
    }

    //Filtros para busca de projetos
    if ($_POST['atidescricao']) {
        $arrWhere[] = "removeacento(atidescricao) ilike(removeacento('%{$_POST['atidescricao']}%'))";
    }
    if ($_POST['atidetalhamento']) {
        $arrWhere[] = "removeacento(atidetalhamento) ilike(removeacento('%{$_POST['atidetalhamento']}%'))";
    }
    if ($_POST['atimetaglobal']) {
        $arrWhere[] = "removeacento(atimetaglobal) ilike(removeacento('%{$_POST['atimetaglobal']}%'))";
    }
    if ($_POST['exoid']) {
        $arrWhere[] = "exoid = {$_POST['exoid']}";
    }
    if ($_POST['atiacaid']) {
        $arrWhere[] = "atiacaid = {$_POST['atiacaid']}";
    }
    if ($_POST['atiexercicio']) {
        $arrWhere[] = "atiexercicio = '{$_POST['atiexercicio']}'";
    } else {
        $arrWhere[] = "(atiexercicio >= " . date('Y') . " OR atiexercicio is null)";
    }

    $sql = "
        select
                a.atiid,
                a.atidescricao,
                a.atiexercicio,
                a.atimetaglobal
        from pde.atividade a
        where
                " . $where . "
                 a.atiestrategico = true and
                 " . ($arrWhere ? implode(" and ", $arrWhere) . " and " : "") . "
                 a.atiidpai is null and
                a.atistatus = 'A' and
                (a.atiid != " . PROJETO_PDE . " AND a.atiid != " . PROJETOENEM . " AND a.atiid != " . PROJETOSEB . " )
        order by
                a.atidescricao
    ";
    $sqlProjeto = $sql;
    $projetos = $db->carregar($sql);

    $projetos = $projetos ? $projetos : array();
    foreach ($projetos as $chave => $projeto) {
        $sql = "select count(*) from pde.atividade where atiidpai = " . $projeto['atiid'] . " and atistatus = 'A'";
        $projetos[$chave]['possui_atividade'] = (boolean) $db->pegaUm($sql);
    }


    function projeto_pegar_gestor($atividade) {
        global $db;
        $sql = sprintf(
            "select
                    u.usunome, u.usucpf, u.usuemail, u.usufoneddd, u.usufonenum,
                    pu.pflcod,
                    un.unidsc
            from seguranca.usuario u
            inner join seguranca.usuario_sistema us on
                    us.usucpf = u.usucpf
            inner join seguranca.perfilusuario pu on
                    pu.usucpf = u.usucpf
            inner join pde.usuarioresponsabilidade ur on
                    ur.usucpf = u.usucpf and ur.pflcod = pu.pflcod
            left join public.unidade un on
                    un.unicod = u.unicod
            where
                    u.suscod = 'A'
                    and us.suscod = 'A' and us.sisid = %d
                    and pu.pflcod = %d
                    and ur.atiid = %d and ur.rpustatus = 'A'
            order by pflcod, usunome", $_SESSION['sisid'], PERFIL_GESTOR, $atividade
        );
        $usuario = $db->recuperar($sql);
        return $usuario ? $usuario : array();
    }

    include APPRAIZ . 'includes/cabecalho.inc';
    print '<br/>';
    $db->cria_aba($abacod_tela, $url, '');
    monta_titulo('Projetos Estratégicos', 'Clique no nome do projeto para iniciar o trabalho.');

?>
<script type="text/javascript">

    function removerProjeto(id, nome) {
        if (confirm('Deseja excluir o projeto \'' + nome + '\'?')) {
            window.location.href = '?modulo=principal/atividade_estrategico/projetos&acao=A&remover=' + id;
        }
    }

    function selecionarProjeto(id) {
        window.location.href = '?modulo=principal/atividade_estrategico/projetos&acao=A&selecionar_projeto=' + id;
    }

    function cadastrarProjeto() {
        var titulo = window.prompt('Título do projeto:', 'Novo Projeto');
        if (titulo) {
            window.location.href = '?modulo=principal/atividade_estrategico/projetos&acao=A&cadastrar_projeto=' + titulo;
        }
    }

    function enviar_email(cpf) {
        var nome_janela = 'janela_enviar_emai_' + cpf;
        window.open(
                '/geral/envia_email.php?cpf=' + cpf,
                nome_janela,
                'width=650,height=557,scrollbars=yes,scrolling=yes,resizebled=yes'
                );
    }

    function pesquisarProjetos(){
        document.getElementById('pesquisarProjeto').submit();
    }

</script>
<form method="post" name="pesquisarProjeto" id="pesquisarProjeto" >
    <input type="hidden" id="requisicao" name="requisicao" value="pesquisarProjeto"/>
    <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="5" align="center" >
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Título:</td>
            <td>
                <?php echo campo_texto('atidescricao', 'N', $permissao, '', 60, 500, '', '', null, null, null, 'id="atidescricao"'); ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
            <td>
                <?php echo campo_texto('atidetalhamento', 'N', $permissao, '', 60, 500, '', '', null, null, null, 'id="atidetalhamento"'); ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" >Eixo:</td>
            <td>
                <?php
                    $sql = "
                        SELECT  exoid AS codigo,
                                exodsc AS descricao
                        FROM painel.eixo
                        ORDER BY descricao
                    ";
                    $db->monta_combo("exoid", $sql, "S", "Selecione...", "", "", "", "200", "N", "", "", $exoid);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >Ação:</td>
            <td>
                <?php
                    $sql = "
                        SELECT  acaid as codigo,
                                acadsc as descricao
                        FROM painel.acao
                        WHERE acastatus = 'A'
                        ORDER BY acadsc
                    ";
                    $db->monta_combo("atiacaid", $sql, "S", "Selecione...", "", "", "", "200", "N", "", "", $atiacaid);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >Ano de Referência:</td>
            <td>
                <?php
                    $sql = "
                        SELECT  DISTINCT atiexercicio as codigo,
                                atiexercicio as descricao
                        FROM pde.atividade
                        WHERE atiexercicio is not null
                        ORDER BY 1
                    ";
                    $db->monta_combo("atiexercicio", $sql, "S", "Selecione...", "", "", "", "", "S", "", "", $atiexercicio)
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Meta:</td>
            <td>
                <?php echo campo_texto('atimetaglobal', 'N', $permissao, '', 60, 500, '', '', null, null, null, 'id="atimetaglobal"'); ?>
            </td>
        </tr>
        <tr>
            <td align="center" bgcolor="#c5c5c5" colspan="2" >
                <input type="button" name="btn_pesquisar" value="Pesquisar" onclick="pesquisarProjetos()" />
                <input type="button" name="btn_vert_todos" value="Ver Todos" onclick="window.location.href = window.location" />
            </td>
        </tr>
    </table>
</form>

<?php if ($usuario_gerente_projeto) : ?>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:none;">
        <tr>
            <td style="padding: 10px;">
                <span style="cursor: pointer" onclick="cadastrarProjeto();" title="novo projeto">
                    <img align="absmiddle" src="/imagens/gif_inclui.gif"/>
                    Cadastrar Projeto Estratégico
                </span>
            </td>
        </tr>
    </table>
<?php endif; ?>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
    <?php if (count($projetos)) : ?>
        <thead>
            <tr bgcolor="#dfdfdf">
                <td align="center" width="70"><b>Ações</b></td>
                <td align="center"><b>Projeto</b></td>
                <td align="center"><b>Ano de Referência</b></td>
                <td align="center"><b>Meta</b></td>
            </tr>
        </thead>
        <?php
            $cor = '';
            foreach ($projetos as $projeto):
                $cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5';
        ?>
                <tr bgcolor="<?= $cor ?>" onmouseout="this.style.backgroundColor = '<?= $cor ?>';" onmouseover="this.style.backgroundColor = '#ffffcc';">
                    <td align="center">

                    <?php if (1 == 1) : ?>
                            <img align="absmiddle" src="/imagens/alterar.gif" style="cursor: pointer" onclick="window.location.href = '?modulo=principal/atividade_estrategico/projeto&acao=A&atiid=<?= $projeto['atiid'] ?>'" title="alterar informações do projeto"/>
                        <?php if (!$projeto['possui_atividade'] && !$boNaoPodeEditar): ?>
                                <img align="absmiddle" src="/imagens/excluir.gif" style="cursor: pointer" onclick="removerProjeto(<?= $projeto['atiid'] ?>, '<?= $projeto['atidescricao'] ?>');" title="excluir projeto"/>
                        <?php else: ?>
                                <img align="absmiddle" src="/imagens/excluir_01.gif"/>
                        <?php endif; ?>
                    <?php else: ?>
                        <img align="absmiddle" src="/imagens/alterar_01.gif"/>
                        <img align="absmiddle" src="/imagens/excluir_01.gif"/>
                    <?php endif; ?>

                    </td>
                    <td>
                        <?PHP if ($atiidENEM && $projeto['atiid'] == $atiidENEM) { ?>
                                <a href="javascript:window.location.href='projetos.php?modulo=principal/atividade_estrategico/listaEnem&acao=A';" title="selecionar projeto">
                        <?PHP } else { ?>
                                <a href="javascript:selecionarProjeto( <?= $projeto['atiid'] ?> );" title="selecionar projeto">
                        <?PHP }

                              echo $projeto['atidescricao']
                        ?>
                                </a>
                        <br>
                        <?PHP
                            $gestor = projeto_pegar_gestor($projeto['atiid']);

                            if ($gestor['usunome']) :
                        ?>
                                <a href="#" onclick="enviar_email('<?= $gestor['usucpf'] ?>');  return false;" style="text-decoration: none;">
                                    <span style="font-size: 9px; font-weight: normal; color: #808080;" title="Clique para enviar e-mail.">
                                            Gestor: <?= $gestor['usunome'] ?>
                                    </span>
                                </a>
                        <?php endif; ?>
                    </td>
                    <td align="center" ><span style="color:blue" ><?= $projeto['atiexercicio'] ?></span></td>
                    <td><?= $projeto['atimetaglobal'] ?></td>
                </tr>
<?php
            endforeach;
        else :
?>
        <tr>
            <td style="text-align:center; padding:15px; background-color:#fafafa; color:#404040; font-weight:bold; font-size: 10px;" colspan="2">
                Não há projetos estratégicos.
            </td>
        </tr>
<?php endif; ?>

</table>