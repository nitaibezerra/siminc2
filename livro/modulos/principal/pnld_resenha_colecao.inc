<script>
    function abrirPopup() {
        window.open('livro.php?modulo=principal/pnld_resenha_colecao&acao=A&colid=popup', 'name', 'height=400,width=400');
    }
</script>

<?php
    header('Content-Type: text/html; charset=iso-8859-1');
    ob_clean();

    $inuid = $_SESSION['rreid'];

    if ($_REQUEST['colid'] == 'popup') {
        $sql = "
            SELECT rreid,
                   arqid,
                   docid,
                   colid,
                   rredescricao,
                   rreusucpf,
                   rrestatus,
                   arqidrecurso,
                   rredescricaorecurso,
                   tpaid,
                   arqidresposta,
                   rredescricaoresposta,
                   arqidparecercondicionado,
                   rredscparecercondicionado
            FROM livro.resenharecurso rre
        ";

        if ($_REQUEST['tipo'] == 'recurso')
            $sql .= "
                INNER JOIN public.arquivo arq ON arq.arqid = rre.arqidrecurso
                WHERE rre.arqidrecurso = " . $_REQUEST['arqid'];
        elseif ($_REQUEST['tipo'] == 'resposta')
            $sql .= "
                INNER JOIN public.arquivo arq ON arq.arqid = rre.arqidresposta
                WHERE rre.arqidresposta = " . $_REQUEST['arqid'];

        $dados = $db->pegaLinha($sql);
        monta_titulo('COMPROVANTE DE ENVIO DO RECURSO', '');
?>
        <div id="noprint">
            <style>
                @media print {
                    #noprint {
                        display: none;
                    }
                    .right {
                        position: absolute;
                        right: 0px;
                        width: 350px;
                    }
                }
            </style>
            <table cellspacing="0" cellpadding="3" border="0" style="width: 80px;">
                <tr style="text-align: center;">
                    <td style="font-size: 7pt; text-align: center;">
                        <a onclick="return false;" alt="Versão para impressão" style="cursor: pointer;">
                            <img onclick="window.print(); return false;" alt="Versão para impressão" src="/imagens/print.png">
                        </a>
                    </td>
                </tr>
            </table>
        </div>

        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css" />

        <table class="tabela"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td class="subtituloDireita">Cód do arquivo enviado</td>
                <td><?php echo $dados['arqid']; ?></td>
            </tr>
            <tr>
                <td class="subtituloDireita">Data de envio</td>
                <td><?php echo $dados['arqdata']; ?></td>
            </tr>
            <tr>
                <td class="subtituloDireita">Hora do Envio</td>
                <td><?php echo $dados['arqhora']; ?></td>
            </tr>
            <tr>
                <td class="subtituloDireita">CPF do Usuário</td>
                <td><?php echo $dados['usucpf']; ?></td>
            </tr>
            <tr>
                <td class="subtituloDireita">Número do arquivo do recurso</td>
                <td><?php echo $dados['arqidrecurso']; ?></td>
            </tr>
            <tr>
                <td class="subtituloDireita">Código do Recurso</td>
                <td><?php echo $dados['rreid']; ?></td>
            </tr>
        </table>
<?PHP
        die();
    }

    if ($_REQUEST['requisicao'] == 'excluirAnexo') {
        $arqid = $_REQUEST['arqid'];
        $colid = $_REQUEST['colid'];
        if ($arqid != '') {
            $sql = " UPDATE livro.resenhaavaliacao SET ravstatus = 'I' WHERE arqid = " . $arqid . "; ";
            $db->executar($sql);
            $db->commit();
            echo "
                <script>
                        alert('Arquivo excluido com sucesso!');
                        window.location.href = 'livro.php?modulo=principal/pnld_resenha_colecao&acao=A&colid=$colid';
                </script>
            ";
            exit();
        }else{
            echo " <script> alert('Ação inválida'); </script> ";
        }
    }


    if ($_REQUEST['req'] == 'excluir') {
        if ($_REQUEST['rreid']) {
            $sql = "
                UPDATE livro.resenharecurso SET
                        rrestatus = 'I'
                WHERE rreid = " . $_REQUEST['rreid'];
            $db->executar($sql);
            $db->commit();
            echo "
                <script>
                    alert('Arquivo excluido!');
                    window.location.href = window.location;
                </script>
            ";
            exit();
        }else{
            echo "
                <script>
                    alert('Ação inválida');
                </script>
            ";
        }
    }
    $colid = $_REQUEST['colid'];

    if ($colid == '') {
        $lvdid = $_REQUEST['lvdid'];
        if ($lvdid == '') {
            echo "
                <script>
                    alert('Escolha um livro.');
                    window.history.back(-1);
                </script>
            ";
            exit;
        }
    }

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    if ($_REQUEST['req'] == 'anexar'){

        $perfis = pegaPerfilGeral();

        if ($colid) {
            if( in_array(PNLD_PERFIL_GESTOR, $perfis) || in_array(PNLD_PERFIL_TECNICO, $perfis) || in_array(PNLD_SUPER_USER, $perfis) ){
                $ravseq = 2;
            }elseif( in_array(PNLD_PERFIL_COORDENADOR, $perfis) ){
                $ravseq = 1;
            }elseif( in_array(PNLD_EDITORA, $perfis) ){
                $ravseq = 3;
            }

            $campos = array(
                "colid" => $colid,
                "ravdescricao" => "'" .$_REQUEST['ravdescricao']."'",
                "tpaid" => $_REQUEST['tpaid'],
                "ravusucpf" => "'".$_SESSION['usucpf']."'",
                "ravstatus" => "'A'",
                "ravseq" => $ravseq
            );

        }else{
            if( in_array(PNLD_PERFIL_GESTOR, $perfis) || in_array(PNLD_PERFIL_TECNICO, $perfis) || in_array(PNLD_SUPER_USER, $perfis) ){
                $ravseq = 2;
            }elseif( in_array(PNLD_PERFIL_COORDENADOR, $perfis) ){
                $ravseq = 1;
            }elseif( in_array(PNLD_EDITORA, $perfis) ){
                $ravseq = 3;
            }

            $campos = array(
                "lvdid" => $lvdid,
                "ravdescricao" => "'".$_REQUEST['ravdescricao']."'",
                "tpaid" => $_REQUEST['tpaid'],
                "ravusucpf" => "'".$_SESSION['usucpf']."'",
                "ravstatus" => "'A'",
                "ravseq" => $ravseq
            );
        }

        $file = new FilesSimec("resenhaavaliacao", $campos, "livro");


        if ($_FILES["arquivo"]){
            $arquivoSalvo = $file->setUpload("arquivo livro", "arquivo");
            if ($arquivoSalvo) {
                echo "
                    <script type=\"text/javascript\">
                        alert('Arquivo anexado.');
                        window.location.href = window.location;
                    </script>
                ";
                exit;
            }
        }
        exit;
    }

    if ($_REQUEST['req'] == 'download'){
        if ($_REQUEST['arqid']) {
            $file = new FilesSimec("resenharecurso", $campos, "livro");
            $file->getDownloadArquivo($_REQUEST['arqid']);
        }
    }

    if ($_REQUEST['req'] == 'final') {
        $sql = "
            SELECT upper(arqextensao) as arqextensao
            FROM public.arquivo
            WHERE arqid = " . $_REQUEST['arqid'];
        $extensao = $db->pegaUm($sql);

        if (trim($extensao) == 'PDF'){
            $sql = "
                UPDATE livro.resenhaavaliacao SET
                        ravseq = 0
                    WHERE ravid = " . $_REQUEST['ravid'];
            $db->executar($sql);

            if($_REQUEST['tpaid'] == 1 || $_REQUEST['tpaid'] == 2 || $_REQUEST['tpaid'] == 3){
                $where = "colid = " . $colid;
                if ($lvdid) {
                    $where = 'lvdid = ' . $lvdid;
                }

                $sql = "
                    SELECT rreid
                    FROM livro.resenharecurso
                    WHERE $where
                ";
                $rreid = $db->pegaUm($sql);

                if (!$rreid){
                    $sql = "
                        INSERT INTO livro.resenharecurso(arqid, colid, rredescricao, rreusucpf, rrestatus, tpaid, lvdid)
                        SELECT arqid, colid, ravdescricao, ravusucpf, ravstatus, tpaid, lvdid
                        FROM livro.resenhaavaliacao
                        WHERE ravid  = " . $_REQUEST['ravid'];
                }else{
                    $sql = "
                        UPDATE livro.resenharecurso SET
                                arqid = " . $_REQUEST['arqid'] . "
                            WHERE rreid = $rreid
                    ";
                }
                $db->executar($sql);
            }
            $db->commit();
        }else{
            echo '
                <script type="text/javascript">
                    alert("Apenas um documento PDF pode ser marcado como versão final.");
                    history.back();
                </script>
            ';
            exit;
        }
    }

    include_once APPRAIZ . 'includes/workflow.php';
    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";
    monta_titulo('Coleções / Livros Regionais', '');

    if ($colid) {
        $w = "col.colid = {$colid}";
        $i = "INNER JOIN livro.colecao col ON col.colid = rre.colid and col.prsano = '" . $_SESSION['exercicio'] . "' ";
    } else {
        $w = "lvd.lvdid = {$lvdid}";
        $i = "INNER JOIN livro.livrodidatico lvd ON lvd.rreid = rre.rreid and lvd.prsano = '" . $_SESSION['exercicio'] . "' ";
    }

    $sql = "
        SELECT rre.tpaid,
               rre.arqid,
               arq.arqnome||'.'||arq.arqextensao as arquivoparecer,
               rre.arqidparecercondicionado,
               rre.rredscparecercondicionado,
               arqcond.arqnome||'.'||arqcond.arqextensao as arquivocondicionado,
               rre.arqidrecurso,
               arqr.arqnome||'.'||arqr.arqextensao as arquivorecurso,
               rre.rredescricaorecurso,
               rre.rredescricao,
               rre.arqidresposta,
               rre.rredescricaoresposta,
               arqresp.arqnome||'.'||arqresp.arqextensao as arquivoresposta
        FROM livro.resenharecurso rre
        {$i}
        INNER JOIN public.arquivo arq ON arq.arqid = rre.arqid
        LEFT  JOIN public.arquivo arqr ON arqr.arqid = rre.arqidrecurso
        LEFT  JOIN public.arquivo arqresp ON arqresp.arqid = rre.arqidresposta
        LEFT  JOIN public.arquivo arqcond ON arqcond.arqid = rre.arqidparecercondicionado
        WHERE {$w} AND  rre.rrestatus = 'A'
    ";

    $resenharecurso = $db->pegaLinha($sql);

    if ($resenharecurso){
        extract($resenharecurso);
    }

    $mostra = "S";
    $st = "ANEXAR PARECER";

    if ($colid){
        $sql = "
            SELECT arqid
            FROM livro.resenharecurso
            WHERE colid = " . $_REQUEST['colid'];
    }else{
        $sql = "
            SELECT rre.arqid
            FROM livro.resenharecurso rre
            INNER JOIN livro.livrodidatico lvd ON lvd.rreid = rre.rreid
            WHERE lvd.lvdid = " . $_REQUEST['lvdid'];
    }
    $arquivo = $db->pegaUm($sql);

    if ($arquivo){
        $st = "UPLOAD/DOWNLOAD DO PARECER";
    }

    if ($tpaid == 3 && in_array(PNLD_EDITORA, arrayPerfil())) {
        $mostra = "N";
        $st = "DOWNLOAD DO PARECER";
    }

    if ($colid){
        $sql = "
            SELECT arqidrecurso
            FROM livro.resenharecurso
            WHERE colid = " . $_REQUEST['colid'];
    }else{
        $sql = "
            SELECT rre.arqidrecurso
            FROM livro.resenharecurso rre
            INNER JOIN livro.livrodidatico lvd ON lvd.rreid = rre.rreid
            WHERE lvd.lvdid = " . $_REQUEST['lvdid'];
    }
    $arquivoRecurso = $db->pegaUm($sql);

    if ($arquivoRecurso){
        $mostra = "N";
    }

    echo cabecalhoLivro($colid, $lvdid);

    $perfis = pegaPerfilGeral();

    if (!$_REQUEST['anexos']){
?>

    <br>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
            <tr>
                <td class="SubTituloEsquerda" colspan="7">
                    Legedas
                </td>
            </tr>
            <tr>
                <td width="2%" align="center">
                    <img border="0" align="absmiddle" src="../imagens/consultar.gif"/>
                </td>
                <td width="8%"> Consultar pareceres </td>
                <td width="2%" align="center">
                   <img border="0" width="18" align="absmiddle" src="../imagens/pnld/download_32x32.png"/>
                </td>
                <td> Download do ultimo Arquivo </td>
            </tr>
        </table>
    <br>

<?PHP
        if( $colid ){
            $sql = "select ravid from livro.resenhaavaliacao where tpaid in (1, 2, 3) and ravseq = 0 and colid = {$colid}";
            $recurso = $db->pegaUm($sql);
        }else{
            $sql = "select ravid from livro.resenhaavaliacao where tpaid in (1, 2, 3) and ravseq = 0 and lvdid = {$lvdid}";
            $recurso = $db->pegaUm($sql);
        }

        if($colid){
            $redirecionar = "
                <a title=\"Ver anexos\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&anexos=1&tpaid=' || tpaid || '&colid=$colid \">
                    <img border=\"0\" align=\"absmiddle\" src=\"../imagens/consultar.gif\" />
                </a>
            ";
        }else{
            $redirecionar = "
                <a title=\"Ver anexos\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&anexos=1&tpaid=' || tpaid || '&lvdid=$lvdid \">
                    <img border=\"0\" align=\"absmiddle\" src=\"../imagens/consultar.gif\" />
                </a>
            ";
        }

        if (in_array(PNLD_PERFIL_GESTOR, $perfis) || in_array(PNLD_SUPER_USER, $perfis)) {
            if($colid){
                $excluir_anexo = "
                    <a title=\"Excluir anexo\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&anexos=1&tpaid=' || tpaid || '&colid=$colid \">
                        <img border=\"0\" align=\"absmiddle\" src=\"../imagens/exclusao.gif\" />
                    </a>
                ";
            }else{
                $excluir_anexo = "
                    <a title=\"Excluir anexo\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&anexos=1&tpaid=' || tpaid || '&lvdid=$lvdid \">
                        <img border=\"0\" align=\"absmiddle\" src=\"../imagens/excluir.gif\" />
                    </a>
                ";
            }
        }else{
            $excluir_anexo = "
                <img title=\"Excluir anexo - Seu perfil não tem permissão para excluir o anexo\" border=\"0\" align=\"absmiddle\" src=\"../imagens/excluir_01.gif\" />
            ";
        }

        monta_titulo('Documentos anexados', '');

        $sql = "
            SELECT  tpa.tpaid,
                    CASE WHEN tpaid IN (1, 2, 3)
                        THEN tpa.tpadsc
                        ELSE tpa.tpadsc
                    END AS tpadsc,
        ";

//        if( $colid != '' && $recurso > 0 ) {
        if( $colid != '' ) {
            $botaoAnexar = "'<center><input type=\"button\" name=\"anexar\" value=\"Anexar\" onClick=\"location.href=\'livro.php?modulo=principal/pnld_resenha_colecao&acao=A&anexos=1&tpaid=' || tpaid || '&colid=$colid\'\" /></center>'";
        }elseif( $lvdid != '' ) {
            $botaoAnexar = "'<center><input type=\"button\" name=\"anexar\" value=\"Anexar\" onClick=\"location.href=\'livro.php?modulo=principal/pnld_resenha_colecao&acao=A&anexos=1&tpaid=' || tpaid || '&lvdid=$lvdid\'\" /></center>'";
        }else{
            $botaoAnexar = "'<center><input type=\"button\" name=\"anexar\" value=\"Anexar\" disabled=\"disabled\" /></center>'";
        }

        $botaoDisabled = "'<center><input type=\"button\" name=\"anexar\" value=\"Anexar\" disabled=\"disabled\" /></center>'";

        #PERMISSÃO - VERIFICA A DATA DE PERMISSÃO PARA ANEXAR OS RECURSOS.
        $permissao = verificaDataAberturaFechamento('R');

        #PERMITIR - VERIFICA A DATA DE ACESSO AO AMBIENTE RESPOSTA E RECURSOS PARA PERFIS COORDENAÇÃO E TECNICO.
        $permitir = verificaDataAberturaFechamento('A');

        $whereColLvd = $colid ? "colid = {$colid}": "lvdid = {$lvdid}";

        if( $permissao == 'S' || $permitir == 'S' ){
                if( !( in_array(PNLD_PERFIL_GESTOR, $perfis) || in_array(PNLD_SUPER_USER, $perfis) ) ){

                    if( ( in_array(PNLD_PERFIL_COORDENADOR, $perfis) || in_array(PNLD_PERFIL_TECNICO, $perfis) ) && $permissao == 'S' ){
                        $anexar = "
                            CASE WHEN tpa.tpaid <> 6 AND tpa.tpaid <> 7 AND tpa.tpaid <> 3
                                THEN {$botaoAnexar}
                                ELSE {$botaoDisabled}
                            END as anexos,
                        ";
                    }elseif( ( in_array(PNLD_EDITORA, $perfis) ) && $permissao == 'S' ){
                        $anexar = "
                            CASE WHEN tpa.tpaid <> 1 AND tpa.tpaid <> 2 AND tpa.tpaid <> 7 AND tpa.tpaid <> 3
                                THEN
                                    CASE WHEN (tpa.tpaid = 6 AND (SELECT COUNT(*) FROM livro.resenhaavaliacao WHERE $whereColLvd AND tpaid = 3 AND ravstatus = 'A') > 0) OR (tpa.tpaid <> 6)
                                        THEN {$botaoAnexar}
                                        ELSE {$botaoDisabled}
                                    END
                                ELSE {$botaoDisabled}
                            END as anexos,
                        ";
                    }else{
                        $anexar = $botaoDisabled.'as anexos,';
                    }

                }else{
                    if (!in_array(PNLD_PERFIL_GESTOR, $perfis)) {
                        $anexar = $botaoAnexar . 'as anexos,';
                    } else {
                        $anexar = "
                            CASE
                                WHEN tpa.tpaid = 7
                                THEN
                                    CASE WHEN (tpa.tpaid = 7 AND (SELECT COUNT(*) FROM livro.resenhaavaliacao WHERE $whereColLvd AND tpaid = 6 AND ravstatus = 'A') > 0) OR (tpa.tpaid <> 7)
                                        THEN {$botaoAnexar}
                                        ELSE {$botaoDisabled}
                                    END

                                WHEN tpa.tpaid = 6
                                THEN
                                    CASE WHEN (tpa.tpaid = 6 AND (SELECT COUNT(*) FROM livro.resenhaavaliacao WHERE $whereColLvd AND tpaid = 3 AND ravstatus = 'A') > 0) OR (tpa.tpaid <> 6)
                                        THEN {$botaoAnexar}
                                        ELSE {$botaoDisabled}
                                    END
                                ELSE {$botaoAnexar}
                            END as anexos,";
                    }
                }
        }else{
            $anexar = $botaoDisabled.'as anexos,';
        }

        if( !( in_array(PNLD_PERFIL_COMISSAO_TECNICA, $perfis) || in_array(PNLD_PERFIL_COORDENADOR, $perfis) ) ){
            #COLEÇÃO
            $down_col_1 = "
                {$redirecionar}<a title=\"Download\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&colid={$colid}&req=download&arqid=' || arqid || '\">
                    <img border=\"0\" width=\"18\" align=\"absmiddle\" src=\"../imagens/pnld/download_32x32.png\" />
                </a>
            ";
            $down_col_2 = "
                {$redirecionar}<a title=\"Download\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&colid={$colid}&req=download&arqid=' || arqid || '\">
                    <img border=\"0\" width=\"18\" align=\"absmiddle\" src=\"../imagens/pnld/download_32x32.png\" />
                </a>
            ";

            #LIVROS;
            $down_liv_1 = "
                {$redirecionar}<a title=\"Download\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&lvdid={$lvdid}&req=download&arqid=' || arqid || '\">
                    <img border=\"0\" width=\"18\" align=\"absmiddle\" src=\"../imagens/pnld/download_32x32.png\" />
                </a>
            ";
            $down_liv_2 = "
                {$redirecionar}<a title=\"Download\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&lvdid={$lvdid}}&req=download&arqid=' || arqid || '\">
                    <img border=\"0\" width=\"18\" align=\"absmiddle\" src=\"../imagens/pnld/download_32x32.png\" />
                </a>
            ";
        }elseif( ( in_array(PNLD_PERFIL_COMISSAO_TECNICA, $perfis) || in_array(PNLD_PERFIL_COORDENADOR, $perfis) ) && $permitir == 'S' ){
            #COLEÇÃO;
            $down_col_1 = "
                {$redirecionar}<a title=\"Download\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&colid={$colid}&req=download&arqid=' || arqid || '\">
                    <img border=\"0\" width=\"18\" align=\"absmiddle\" src=\"../imagens/pnld/download_32x32.png\" />
                </a>
            ";
            $down_col_2 = "
                {$redirecionar}<a title=\"Download\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&colid={$colid}&req=download&arqid=' || arqid || '\">
                    <img border=\"0\" width=\"18\" align=\"absmiddle\" src=\"../imagens/pnld/download_32x32.png\" />
                </a>
            ";

            #LIVROS
            $down_liv_1 = "
                {$redirecionar}<a title=\"Download\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&lvdid={$lvdid}&req=download&arqid=' || arqid || '\">
                    <img border=\"0\" width=\"18\" align=\"absmiddle\" src=\"../imagens/pnld/download_32x32.png\" />
                </a>
            ";
            $down_liv_2 = "
                {$redirecionar}<a title=\"Download\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&lvdid={$lvdid}}&req=download&arqid=' || arqid || '\">
                    <img border=\"0\" width=\"18\" align=\"absmiddle\" src=\"../imagens/pnld/download_32x32.png\" />
                </a>
            ";
        }else{
            #COLEÇÃO
            $down_col_1 = "
                <img border=\"0\" align=\"absmiddle\" src=\"../imagens/consultar_01.gif\" />
                <a title=\"Download\"><img border=\"0\" width=\"18\" align=\"absmiddle\" src=\"../imagens/pnld/download_disabled_32x32.png\" /></a>
            ";
            $down_col_2 = "
                {$redirecionar}<a title=\"Download\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&colid={$colid}&req=download&arqid=' || arqid || '\">
                    <img border=\"0\" width=\"18\" align=\"absmiddle\" src=\"../imagens/pnld/download_32x32.png\" />
                </a>
            ";

            #LIVROS
            $down_liv_1 = "
                <img border=\"0\" align=\"absmiddle\" src=\"../imagens/consultar_01.gif\" />
                <a title=\"Download\"><img border=\"0\" width=\"18\" align=\"absmiddle\" src=\"../imagens/pnld/download_disabled_32x32.png\" /></a>
            ";
            $down_liv_2 = "
                {$redirecionar}<a title=\"Download\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&lvdid={$lvdid}&req=download&arqid=' || arqid || '\">
                    <img border=\"0\" width=\"18\" align=\"absmiddle\" src=\"../imagens/pnld/download_32x32.png\" />
                </a>
            ";
        }

        if ($colid) {
            $sql .= "
                {$anexar}
                CASE
                    WHEN tpa.tpaid = 6
                        THEN (
                            SELECT CAST('{$down_col_1}' AS text)
                            FROM livro.resenhaavaliacao
                            WHERE colid = {$colid} AND tpaid = 6 AND ravstatus = 'A'
                            ORDER BY ravid DESC
                            LIMIT 1
                        )
                    WHEN tpa.tpaid = 7
                        THEN (
                            SELECT CAST('{$down_col_1}' AS text)
                            FROM livro.resenhaavaliacao
                            WHERE colid = {$colid} AND tpaid = 7 AND ravstatus = 'A'
                            ORDER BY ravid DESC
                            LIMIT 1
                        )
                    ELSE
                        COALESCE(
                            (
                                SELECT CAST('{$down_col_2}' AS text)
                                FROM livro.resenhaavaliacao
                                WHERE colid = {$colid} AND (tpaid = tpa.tpaid AND tpaid <> 7) AND ravseq = 0 AND ravstatus = 'A'
                            ),(
                                SELECT CAST('{$down_col_2}' AS text)
                                FROM livro.resenhaavaliacao
                                WHERE colid = {$colid} AND (tpaid = tpa.tpaid AND tpaid <> 7) AND ravstatus = 'A'
                                ORDER BY ravseq DESC
                                LIMIT 1
                            )
                        )
                END as acao
            ";
        }else{
            $sql .= "
                {$anexar}
                CASE
                    WHEN tpa.tpaid = 6
                        THEN (
                            SELECT CAST('{$down_liv_1}' AS text)
                            FROM livro.resenhaavaliacao
                            WHERE lvdid = {$lvdid} AND tpaid = 6 AND ravstatus = 'A'
                            ORDER BY ravid DESC
                            LIMIT 1
                        )
                    WHEN tpa.tpaid = 7
                        THEN (
                            SELECT CAST('{$down_liv_1}' AS text)
                            FROM livro.resenhaavaliacao
                            WHERE lvdid = {$lvdid} AND tpaid = 7 AND ravstatus = 'A'
                            ORDER BY ravid DESC
                            LIMIT 1
                        )
                    ELSE
                        COALESCE(
                            (
                                SELECT CAST('{$down_liv_2}' AS text)
                                FROM livro.resenhaavaliacao
                                WHERE lvdid = {$lvdid} AND tpaid = tpa.tpaid AND ravseq = 0 AND ravstatus = 'A'
                            ),(
                                SELECT CAST('{$down_liv_2}' AS text)
                                FROM livro.resenhaavaliacao
                                WHERE lvdid = {$lvdid } AND tpaid = tpa.tpaid AND ravstatus = 'A'
                                ORDER BY ravseq DESC LIMIT 1
                            )
                        )
                END as acao
            ";
        }

    	if( in_array(PNLD_EDITORA, $perfis) ){
            $whereEditora = ' and tpa.tpaid not in (5) ';
        }
        $sql .= "
            FROM livro.tipoparecer tpa
            WHERE tpa.tpastatus = 'A' {$whereEditora}
            ORDER BY tpa.tpaid
        ";
        $cabecalho = array('Item', 'Descrição', 'Anexos', 'Ação');
        $whidth = Array('5%', '60%', '20%', '5%');
        $align  = Array('center', 'left', 'center', 'center');
//        echo "<pre>";
//        echo $sql;
//        echo "</pre>";

        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $whidth, $align, '');

       if( !in_array(PNLD_EDITORA, $perfis) ){
            echo "<center><input type=\"button\" name=\"voltar\" value=\"Voltar\" onClick=\"location.href='livro.php?modulo=principal/pnld_editora&acao=A'\" /></center>";
        }else{
            echo "<center><input type=\"button\" name=\"voltar\" value=\"Voltar\" onClick=\"location.href='livro.php?modulo=principal/pnld_editora_analise&acao=A'\" /></center>";
        }
    } else {
    ?>
    <script type="text/javascript">

        function abrirAvaliacaoMidiaColecao( ravid, colid ){
            window.open('livro.php?modulo=principal/pnld_parecer_midia&acao=A&ravid='+ravid+'&colid='+colid,'','toolbar=no, location=no, status=yes, menubar=no, scrollbars=yes, resizable=no, width=500, height=215');
        }

        function anexarArquivo(tipo) {
            var req  = document.getElementById('req');
            var form = document.getElementById('form_resenha');

            if (document.getElementById('arquivo').value == '') {
                alert('Por favor selecione um arquivo no formato PDF.');
                return false;
            }
            if (form.ravdescricao.value == '') {
                alert('Por favor informe a descrição do arquivo.');
                return false;
            }
            if( trim(tipo) != 'editora' ){
                if (!verificaExtensao('arquivo')){
                    return false;
                }
            }else{
                if (!verificaExtensaoEditora('arquivo')){
                    return false;
                }
            }
            req.value = 'anexar';
            form.submit();
        }

        function verificaExtensao(id) {
            arquivo = document.getElementById(id);
            if (arquivo.value != '') {
                arFile = arquivo.value.split('.');
                indice = arFile.length - 1;
                extensao = arFile[indice].toLowerCase();
                if (extensao != 'pdf' && extensao != 'doc' && extensao != 'docx' && extensao != 'odt') {
                    alert('Extensão não permitida! Envie apenas arquivo com extensão PDF, DOC e DOCX.');
                    arquivo.value = '';
                    return false;
                }
            }
            return true;
        }

        function verificaExtensaoEditora(id) {
            arquivo = document.getElementById(id);
            if (arquivo.value != '') {
                arFile = arquivo.value.split('.');
                indice = arFile.length - 1;
                extensao = arFile[indice].toLowerCase();
                if (extensao != 'pdf') {
                    alert('Extensão não permitida! Envie apenas arquivo com extensão PDF.');
                    arquivo.value = '';
                    return false;
                }
            }
            return true;
        }

    </script>
<?php
        $sql = "
            SELECT tpadsc
            FROM livro.tipoparecer
            WHERE tpaid = " . $_REQUEST['tpaid'];
        $titulo = $db->pegaUm($sql);
        monta_titulo($titulo, '');

        $sql = "
            SELECT ravid
            FROM livro.resenhaavaliacao
            WHERE ravseq = 0
            AND ravstatus = 'A'
            AND   tpaid  = " . $_REQUEST['tpaid'];

    if ($colid){
        $sql .= " AND colid = $colid ";
    }else{
        $sql .= " AND lvdid = $lvdid ";
    }

    $finalizado = $db->pegaUm($sql);

    //if (!$finalizado and $_REQUEST['tpaid'] != 6) {
    if (!$finalizado) {

    	if( in_array(PNLD_EDITORA, $perfis) ){
            if ($_REQUEST['colid']) {
                $total = $db->pegaUm("SELECT count(arqid) FROM livro.resenhaavaliacao WHERE colid = {$_REQUEST['colid']} and tpaid = {$_REQUEST['tpaid']} and ravstatus = 'A'");
            } else {
                $total = $db->pegaUm("SELECT count(arqid) FROM livro.resenhaavaliacao WHERE lvdid = {$_REQUEST['lvdid']} and tpaid = {$_REQUEST['tpaid']} and ravstatus = 'A'");
            }
            if($total > 0){
                $boAtivaAnexos = 'N';
            } else {
                $boAtivaAnexos = 'S';
            }
    	} else {
            $boAtivaAnexos = 'S';
    	}
?>
    <form action="" method="post" name="form_resenha" id="form_resenha" enctype="multipart/form-data">
        <input type="hidden" name="req" id="req" value="" />
        <input type="hidden" name="rreid" id="rreid" value="" />
        <input type="hidden" name="colid" id="colid" value="<?=$colid;?>" />
        <input type="hidden" name="lvdid" id="lvdid" value="<?=$lvdid;?>" />
        <input type="hidden" name="tpaid" id="tpaid" value="<?=$_REQUEST['tpaid'];?>" />
        <input type="hidden" id="arqRecurso" name="arqRecurso" value="<?=$arquivoRecurso;?>">

        <table width="95%" class="tabela" align="center" border="0" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
            <tr>
                <td>
                    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                        <tr>
                            <td class="SubTituloDireita" width="25%"><b>Arquivo <?= $titulo ?></b></td>
                            <td style="border-bottom: 1px solid #DCDCDC">
                                <input type="file" name="arquivo" id="arquivo" <?=($boAtivaAnexos == 'N' ? 'disabled="disabled"': ''); ?> />
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita"><b>Descrição:</b></td>
                            <td style="border-bottom: 1px solid #DCDCDC">
                                <?php echo campo_texto('ravdescricao', 'S', $boAtivaAnexos, '', 40, 40, '', '', '', '', '', 'ravdescricao'); ?>
                            </td>
                        </tr>
                        <tr bgcolor="#DCDCDC">
                            <td></td>
                            <td colspan="2" align="left">
                                <?php
                                    #*****************************************************************************************#
                                    # CÓDIGO COMENTADO, PORQUE ATÉ A ESSA DATA ESPECIFICA, 12/04/2013, ESTA SEM NENHUMA AÇÃO. #
                                    # ESSA PARTE DO CÓDIGO FICA ÁPOS O PRIMEIRO "IF" FAZENDO UMA SEGUNDO BLOCO CONDICIONAL    #
                                    # if(date('Y-m-d')<DATA_ENCERRAMENTO_RECURSO || !in_array(PNLD_EDITORA, arrayPerfil())) { #
                                    #   if (!($arqidresposta && $_REQUEST['analise'])) {                                      #
                                    #*****************************************************************************************#

                                    $permissao = verificaDataAberturaFechamento('R');

                                    if( ( in_array(PNLD_PERFIL_GESTOR, $perfis) || in_array(PNLD_PERFIL_COORDENADOR, $perfis) || in_array(PNLD_PERFIL_TECNICO, $perfis) || in_array(PNLD_SUPER_USER, $perfis) ) ){
                                        echo "<input type=\"button\" value=\"Anexar\" onclick=\"anexarArquivo('normal');\" />";
                                    //}elseif( ( date('Y-m-d') < DATA_ENCERRAMENTO_RECURSO ) && $boAtivaAnexos == 'S' && in_array(PNLD_EDITORA, $perfis) ){
                                    }elseif( ( $permissao == 'S' ) && $boAtivaAnexos == 'S' && in_array(PNLD_EDITORA, $perfis) ){
                                        echo "<input type=\"button\" value=\"Anexar\" onclick=\"anexarArquivo('editora');\" />";
                                    }else{
                                        echo "<input type=\"button\" value=\"Anexar\" disabled=\"disabled\" />";
                                    }
                                ?>
                                <input type="button" value="Voltar" onclick="window.location = 'livro.php?modulo=principal/pnld_resenha_colecao&acao=A&<?php if ($colid) echo 'colid=' . $colid; else echo 'lvdid=' . $lvdid; ?>'">
                            </td>
                        </tr>
                    </table>
                </td>
            </tr>
        </table>
    </form>
<?php

    }else{

?>
    <br>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
        <tr>
            <td class="SubTituloEsquerda" colspan="7">
                Legedas
            </td>
        </tr>
        <tr>
            <td width="2%" align="center">
                <img border="0" align="absmiddle" src="../imagens/excluir.gif"/>
            </td>
            <td width="8%"> Excluir parecer </td>
            <td width="2%" align="center">
                <img border="0" align="absmiddle" src="../imagens/pnld/parecer_media_16x16.png"/>
            </td>
            <td width="10%"> Definir paracer da mídia </td>
            <td width="2%" align="center">
                <img border="0" align="absmiddle" src="../imagens/check_p.gif"/>
            </td>
            <td> Versão final </td>
        </tr>
    </table>
    <br>

<?php
    }
    monta_titulo('Documentos anexados', '');

    $perfis = pegaPerfilGeral();


    $acao_final_tipo_1 = "
        <img border=\"0\" align=\"absmiddle\" style=\"cursor: pointer;\" src=\"../imagens/pnld/parecer_media_16x16.png\" onclick=\"abrirAvaliacaoMidiaColecao('|| r.ravid  ||','|| r.colid ||');\"/>
    ";

    $acao_final_tipo_2 = "
        <img title=\"Marcar como versão final\" border=\"0\" align=\"absmiddle\" src=\"../imagens/check_p.gif\" />
    ";

    $acao_sem_finalizar = "
        <a title=\"Excluir anexo\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&colid=$colid&requisicao=excluirAnexo&arqid='|| r.arqid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/excluir.gif\" /></a>
    ";

    if (in_array(PNLD_PERFIL_GESTOR, $perfis) || in_array(PNLD_SUPER_USER, $perfis)){
        $excluir_anexo = "
            CASE WHEN lvd.cpsid = 1 AND (select count(tpm_id) from livro.resenhaavaliacao where colid = r.colid and tpaid <> r.tpaid) = 0
                THEN
                    CASE WHEN ravseq = 0
                        THEN '{$acao_final_tipo_1}'
                        ELSE '{$acao_sem_finalizar}'
                    END
                ELSE
                    CASE WHEN ravseq = 0
                        THEN '{$acao_final_tipo_2}'
                        ELSE '{$acao_sem_finalizar}'
                    END
            END as acao,
	";
    }else{
        $excluir_anexo = "
            CASE WHEN  ravseq = 0
                THEN ''
		ELSE '<center><img title=\"Excluir anexo - Seu perfil não tem permissão para excluir o anexo\" border=\"0\" align=\"absmiddle\" src=\"../imagens/excluir_01.gif\" /></center>'
            END as acao,
	";
    }

    //if ($_REQUEST['tpaid'] != 6) {
    if ($_REQUEST['tpaid']) {
        $campo_versao = "
            CASE WHEN ravseq = 0
                THEN '<center><img border=\"0\" align=\"absmiddle\" src=\"../imagens/check_p.gif\" /></center>'
                ELSE ''
            END AS versaoFinal
	";

        if ($colid){
            if (in_array(PNLD_PERFIL_GESTOR, $perfis) || in_array(PNLD_SUPER_USER, $perfis)) {
                $imgLink_col = "
                    '<center>
                        <a title=\"Marcar como versão final\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&anexos=" . $_REQUEST['anexos'] . "&tpaid=" . $_REQUEST['tpaid'] . "&colid=$colid&req=final&ravid=' || ravid || '&arqid=' || arqid || '\">
                            <img border=\"0\" align=\"absmiddle\" src=\"../imagens/check_p_01.gif\" />
                        </a>
                     </center>'";
            } else {
                $imgLink_col = "'<center><img title=\"Marcar como versão final\" border=\"0\" align=\"absmiddle\" src=\"../imagens/check_p_01.gif\" /></center>'";
            }

            $campo_arquivo = "'<a title=\"Download\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&colid=$colid&req=download&arqid=' || arqid || '\">' || ravdescricao || '</a>'";

            if (!$finalizado)
                $campo_versao = "
                    CASE WHEN ravid = (
                            SELECT ravid
                            FROM livro.resenhaavaliacao
                            WHERE colid = $colid AND tpaid = ".$_REQUEST['tpaid']." and ravstatus = 'A'
                            ORDER BY ravid DESC
                            LIMIT 1)
                        THEN $imgLink_col
                        ELSE ''
                    END AS versaoFinal
                ";
            $where = "r.colid = ".$colid;
        }else{
            if (in_array(PNLD_PERFIL_GESTOR, $perfis) || in_array(PNLD_SUPER_USER, $perfis)) {
                $imgLink = "
                    '<center>
                        <a title=\"Marcar como versão final\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&anexos=" . $_REQUEST['anexos'] . "&tpaid=" . $_REQUEST['tpaid'] . "&lvdid=$lvdid&req=final&ravid=' || ravid || '&arqid=' || arqid || '\">
                            <img border=\"0\" align=\"absmiddle\" src=\"../imagens/check_p_01.gif\" />
                        </a>
                    </center>'
                ";
            } else {
                $imgLink = "
                    '<center>
                        <img title=\"Marcar como versão final\" border=\"0\" align=\"absmiddle\" src=\"../imagens/check_p_01.gif\" />
                    </center>'
                ";
            }

            $campo_arquivo = "'<a title=\"Download\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&lvdid=$lvdid&req=download&arqid=' || arqid || '\">' || ravdescricao || '</a>'";
            if (!$finalizado)
                $campo_versao = "
                    CASE WHEN ravid = (
                            SELECT ravid
                            FROM livro.resenhaavaliacao
                            WHERE lvdid = $lvdid AND tpaid = ".$_REQUEST['tpaid']." and ravstatus = 'A'
                            ORDER BY ravid DESC
                            LIMIT 1)
                        THEN $imgLink
                        ELSE ''
                    END AS versaoFinal
                ";
            $where = "lvd.lvdid = ".$lvdid;
        }

        if (in_array(PNLD_EDITORA, $perfis) ){
        	$whereEditora = ' and ravseq in ( 0, 1, 2, 3 )';
        }


        if($colid){
            $join = "   JOIN livro.colecao col ON col.colid = r.colid
                        JOIN livro.editora edt ON edt.edtid = col.edtid AND edt.edtstatus = 'A'
                        JOIN livro.livrodidatico lvd ON lvd.edtid = edt.edtid AND lvd.colid = col.colid";
        } else {
            $join = " JOIN livro.livrodidatico lvd ON lvd.lvdid = r.lvdid ";
        }
        
        $sql = "
            SELECT  DISTINCT $excluir_anexo
                    CASE WHEN ravseq = 2
                        THEN 'EQUIPE MEC - GESTORES / TÉCNICOS'
                        ELSE
                            CASE WHEN ravseq = 1
                                THEN 'UNIVERSIDADE - COORDENADORES'
                                ELSE
                                    CASE WHEN ravseq = 3
                                        THEN 'EDITORA'
                                        ELSE ''
                                    END
                            END
                    END as ravseq,
                    $campo_arquivo,
                    $campo_versao
            FROM livro.resenhaavaliacao AS r
            $join
            WHERE $where AND tpaid = ".$_REQUEST['tpaid']." {$whereEditora} and ravstatus = 'A'

            ORDER by 1 ASC
        ";
//        ver($sql, d);
        $cabecalho  = array('Ação', 'Responsável pelo anexo', 'Descrição', 'Versão final');
        $whidth     = Array('5%', '20%', '65%', '5%');
        $align      = Array('center', 'left', 'left', 'center');
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $whidth, $align, '');
    }else{
        if($colid){
            $campo_arquivo = "
                CASE WHEN rredescricao <> ''
                    THEN '<a title=\"Download\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&colid=$colid&req=download&arqid=' || arqid || '\">' || rredescricao || '</a>'
                    ELSE '<a title=\"Download\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&colid=$colid&req=download&arqid=' || arqid || '\">Recurso</a>'
                END AS descricao
            ";
            $where = "colid = ".$colid;
        } else {
            $campo_arquivo = "
                CASE WHEN rredescricao <> ''
                    THEN '<a title=\"Download\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&lvdid=$lvdid&req=download&arqid=' || arqid || '\">' || rredescricao || '</a>'
                    ELSE '<a title=\"Download\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&lvdid=$lvdid&req=download&arqid=' || arqid || '\">Recurso</a>'
                END AS descricao
            ";
            $where = "lvdid = " . $lvdid;
        }

        $sql = "
            SELECT $campo_arquivo
            FROM livro.resenharecurso
            WHERE $where
	";
        $cabecalho = array('Descrição');
        $db->monta_lista($sql, $cabecalho, 20, 20, 'N', '70%');
    }
?>

    <table width="95%" class="tabela" align="center" border="0" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td align="center">
                <input type="button" value="Voltar" onclick="window.location = 'livro.php?modulo=principal/pnld_resenha_colecao&acao=A&<?php if ($colid) echo 'colid=' . $colid; else echo 'lvdid=' . $lvdid; ?>'">
            </td>
        </tr>
    </table>

<?PHP
    }
?>