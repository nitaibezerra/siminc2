<?PHP
    if($_SESSION['maismedicomec']['mntid'] == ''){
        $db->sucesso("principal/mantenedora/cad_mantenedora", '', "Ocorreu um problema, é necessário o preenchimento dos dados da Mantida. Tente novamente!");
    }

    verificaMantenedora();

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    if($_REQUEST['requisicao'] == 'download'){
        $arqid = $_REQUEST['arqid'];
        $file = new FilesSimec();
        $arquivo = $file->getDownloadArquivo($arqid);
        echo"<script>window.location.href = '".$_SERVER['HTTP_REFERER']."';</script>";
        exit;
    }

    if($_REQUEST['requisicao'] == 'delete'){
        if( $_REQUEST['arqid'] ){
            $arqid = $_REQUEST['arqid'];
            $sql = "
                DELETE FROM maismedicomec.arqparecermantida WHERE arqid = {$arqid};
                UPDATE public.arquivo SET arqstatus='I' WHERE arqid = {$arqid}
            ";
            $db->executar($sql);
        }
        $db->commit();

        #REMOVE O REGISTRO DO ANEXO ANTIGO (PELO ARQID) E O ARQUIVO FÍSICO
        $file = new FilesSimec();
        $file->excluiArquivoFisico($arqid);

        die("
            <script>
                alert('Arquivo excluído com sucesso!');
                location.href='maismedicomec.php?modulo=principal/mantenedora/parecer&acao=A&mtdid=".$mtdid."';
            </script>
        ");
    }

    if($_REQUEST['requisicao'] == 'inserir') {

        if($_REQUEST['tipo'] == '1') {

            if ($_FILES['arqidmec']['size']) {
                $arqid = EnviarArquivo($_FILES['arqidmec']);

                if(!$arqid){
                    die( "<script> alert(\"Problemas no envio do arquivo.\"); history.go(-1); </script>" );
                }
            }

            if(!$_REQUEST['pimid']) {
                $sql = "
                    INSERT INTO maismedicomec.parecerinscricaomantida(
                            mnmid, pimdscparecer, pimdtinclusaomec, usucpfmec, mntstatus
                        ) VALUES (
                            {$_REQUEST['mtdid']}, '{$_REQUEST['pimdscparecer']}', now(), '{$_SESSION['usucpf']}', 'A'
                    ) RETURNING pimid
                ";
            }else{
                $sql = "
                    UPDATE maismedicomec.parecerinscricaomantida
                        SET pimdscparecer       = '{$_REQUEST['pimdscparecer']}',
                            pimdtinclusaomec    = now(),
                            usucpfmec           = '{$_SESSION['usucpf']}'
                        WHERE pimid = {$_REQUEST['pimid']} RETURNING pimid
                ";
            }
            $pimid = $db->pegaUm($sql);

            if( $arqid > 0 && $pimid > 0 ){
                #O UPDATE REALIZADO NO CAMPO arqidmec É FEITO APENAS PARA CONTROLE. SE O CAMPO NÃO FOR NULO SABE-SE QUE A UM ARQUIVO ANEXADO AO PARECER.
                #OS ARQUIVOS ANEXADOS SÃO IDENTIFICADOS NA TABELA arqparecermantida
                $sql = "
                    INSERT INTO maismedicomec.arqparecermantida(arqid, pimid, apmtipo) VALUES ({$arqid}, {$pimid}, 'M');
                    UPDATE maismedicomec.parecerinscricaomantida SET arqidmec = {$arqid} WHERE pimid = {$pimid};
                ";
                $db->executar($sql);
            }
        }

        if($_REQUEST['tipo'] == '2') {

            if ($_FILES['arqidies']['size']) {
                $arqid = EnviarArquivo($_FILES['arqidies']);

                if(!$arqid){
                    die("<script> alert(\"Problemas no envio do arquivo.\"); history.go(-1); </script>");
                }
            }

            if(!$_REQUEST['pimid']) {
                $sql = "
                    INSERT INTO maismedicomec.parecerinscricaomantida(
                            mnmid, pimdscresposta, pimdtinclusaoies, usucpfies, mntstatus
                        ) VALUES (
                            {$_REQUEST['mtdid']}, '{$_REQUEST['pimdscresposta']}', now(), '{$_SESSION['usucpf']}', 'A'
                    ) RETURNING pimid
                ";
            }else{
                $sql = "
                    UPDATE maismedicomec.parecerinscricaomantida
                        SET pimdscresposta      = '{$_REQUEST['pimdscresposta']}',
                            pimdtinclusaoies    = now(),
                            usucpfies           = '{$_SESSION['usucpf']}'
                    WHERE pimid = {$_REQUEST['pimid']} RETURNING pimid
                ";
            }
            $pimid = $db->pegaUm($sql);

            if( $arqid > 0 && $pimid > 0 ){
                #O UPDATE REALIZADO NO CAMPO arqidies É FEITO APENAS PARA CONTROLE. SE O CAMPO NÃO FOR NULO SABE-SE QUE A UM ARQUIVO ANEXADO AO PARECER.
                #OS ARQUIVOS ANEXADOS SÃO IDENTIFICADOS NA TABELA arqparecermantida
                $sql = "
                    INSERT INTO maismedicomec.arqparecermantida(arqid, pimid, apmtipo) VALUES ({$arqid}, {$pimid}, 'R');
                    UPDATE maismedicomec.parecerinscricaomantida SET arqidies = {$arqid} WHERE pimid = {$pimid};
                ";
                $db->executar($sql);
            }
        }

        if($_REQUEST['tipo'] == '3') {

            if ($_FILES['arqanaliserec']['size']) {
                $arqid = EnviarArquivo($_FILES['arqanaliserec']);

                if(!$arqid){
                    die("<script> alert(\"Problemas no envio do arquivo.\"); history.go(-1); </script>");
                }
            }

            $pimopcaorecurso = $_REQUEST['pimopcaorecurso'] == 'D' ? 't' : 'f';

            if(!$_REQUEST['pimid']){
                $sql = "
                    INSERT INTO maismedicomec.parecerinscricaomantida(
                            mnmid, pimdscanaliserec, pimdtanaliserec, usucpfanaliserec, pimopcaorecurso, mntstatus
                        ) VALUES (
                            {$_REQUEST['mtdid']}, '{$_REQUEST['pimdscanaliserec']}', now(), '{$_SESSION['usucpf']}', '{$pimopcaorecurso}', 'A'
                    ) RETURNING pimid
                ";
            }else{
                $sql = "
                    UPDATE maismedicomec.parecerinscricaomantida
                        SET pimdscanaliserec    = '{$_REQUEST['pimdscanaliserec']}',
                            pimdtanaliserec     = now(),
                            usucpfanaliserec    = '{$_SESSION['usucpf']}',
                            pimopcaorecurso     = '{$pimopcaorecurso}'
                    WHERE pimid = {$_REQUEST['pimid']} RETURNING pimid
                ";
            }
            $pimid = $db->pegaUm($sql);

            if( $arqid > 0 && $pimid > 0 ){
                #O UPDATE REALIZADO NO CAMPO arqanaliserec É FEITO APENAS PARA CONTROLE. SE O CAMPO NÃO FOR NULO SABE-SE QUE A UM ARQUIVO ANEXADO AO PARECER.
                #OS ARQUIVOS ANEXADOS SÃO IDENTIFICADOS NA TABELA arqparecermantida
                $sql = "
                    INSERT INTO maismedicomec.arqparecermantida(arqid, pimid, apmtipo) VALUES ({$arqid}, {$pimid}, 'P');
                    UPDATE maismedicomec.parecerinscricaomantida SET arqanaliserec = {$arqid} WHERE pimid = {$pimid};
                ";
                $db->executar($sql);
            }
        }
        $db->commit();

        die("
            <script>
                alert('Operação realizada com sucesso!');
                location.href='maismedicomec.php?modulo=principal/mantenedora/parecer&acao=A&mtdid=".$_REQUEST['mtdid']."';
            </script>
        ");
    }


    function EnviarArquivo($arquivo){
        global $db;

        if(!$arquivo){
            die("<script> alert(\"Problemas no envio do arquivo.\"); history.go(-1); </script>");
        }

        // BUG DO IE
        // O type do arquivo vem como image/pjpeg
        if($arquivo["type"] == 'image/pjpeg') {
            $arquivo["type"] = 'image/jpeg';
        }

        // Corrigindo erro de referência do php.
        $dadosArquivo = explode(".", $arquivo["name"]);

        //Insere o registro do arquivo na tabela public.arquivo
        $sql = "
            INSERT INTO public.arquivo( arqnome, arqextensao, arqdescricao, arqtipo, arqtamanho, arqdata, arqhora, usucpf, sisid
                )VALUES(
                    '".current($dadosArquivo)."', '".end($dadosArquivo)."', NULL, '{$arquivo["type"]}', '{$arquivo["size"]}', '".date('Y/m/d')."', '".date('H:i:s')."', '{$_SESSION["usucpf"]}', {$_SESSION["sisid"]}
            ) RETURNING arqid
        ";
        $arqid = $db->pegaUm($sql);

        if(!is_dir('../../arquivos/maismedicomec/'.floor($arqid/1000))) {
            mkdir(APPRAIZ.'/arquivos/maismedicomec/'.floor($arqid/1000), 0777, true);
        }

        $caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;

        switch($arquivo["type"]) {
            case 'image/jpeg':
                ini_set("memory_limit", "128M");
                list($width, $height) = getimagesize($arquivo['tmp_name']);
                $original_x = $width;
                $original_y = $height;
                // se a largura for maior que altura
                if($original_x > $original_y) {
                    $porcentagem = (100 * 640) / $original_x;
                }else {
                    $porcentagem = (100 * 480) / $original_y;
                }
                $tamanho_x = $original_x * ($porcentagem / 100);
                $tamanho_y = $original_y * ($porcentagem / 100);
                $image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
                $image   = imagecreatefromjpeg($arquivo['tmp_name']);
                imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
                imagejpeg($image_p, $caminho, 100);
                //Clean-up memory
                ImageDestroy($image_p);
                //Clean-up memory
                ImageDestroy($image);
                break;
            default:
                if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
                    $db->rollback();
                    return false;
                }
        }

        $db->commit();
        return $arqid;
    }

    function listagemArquivosParecer( $pimid, $habilita, $tp_parecer ){
        global $db;

        if( $habilita == 'S' ){
            $acao = "
                <center>
                    <a href=\"maismedicomec.php?modulo=principal/mantenedora/parecer&acao=A&arqid=' || a.arqid || '&requisicao=download\">
                        <img border=0 src=\"../imagens/anexo.gif\" />
                    </a>
                    <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'maismedicomec.php?modulo=principal/mantenedora/parecer&acao=A&arqid=' || a.arqid || '&requisicao=delete\');\" >
                        <img border=0 src=\"../imagens/excluir.gif\" />
                    </a>
                </center>
            ";
        }else{
            $acao = "
                <center>
                    <a href=\"maismedicomec.php?modulo=principal/mantenedora/parecer&acao=A&arqid=' || a.arqid || '&requisicao=download\">
                        <img border=0 src=\"../imagens/anexo.gif\" />
                    </a>
                </center>
            ";
        }

        $sql = "
            SELECT  '{$acao}' AS acao,
                    '<center>'||to_char(a.arqdata,'DD/MM/YYYY')||'</center>' AS dtinclusao,
                    '<a style=\"cursor: pointer; color: blue;\" href=\"maismedicomec.php?modulo=principal/mantenedora/parecer&acao=A&arqid=' || a.arqid || '&requisicao=download\">' || a.arqnome || '</a>' AS arqnome,
                    a.arqtamanho || ' kb' AS arqtamanho,
                    u.usunome
            FROM public.arquivo a

            LEFT JOIN seguranca.usuario AS u ON u.usucpf = a.usucpf
            JOIN maismedicomec.arqparecermantida AS am ON am.arqid = a.arqid

            WHERE am.apmtipo = '{$tp_parecer}' AND am.pimid = {$pimid}
        ";
        $cabecalho = array("Ação", "Data da Inclusão", "Nome do Arquivo", "Tamanho", "Responsável");
        $db->monta_lista_simples($sql, $cabecalho, 10, 10, 'N', '', '');
    }

    include APPRAIZ . "includes/cabecalho.inc";
    monta_titulo( 'Mais Médicos - MEC', 'Resumo' );

    $abacod_tela    = ABA_CADASTRO_MANTENEDORA;
    $url            = 'maismedicomec.php?modulo=principal/mantenedora/parecer&acao=A';
    $parametros     = '';

    $db->cria_aba($abacod_tela, $url, $parametros);

    //busca dados para edição
    if($_REQUEST['mtdid']) {
        $sql = "
            SELECT pimid, arqidmec, pimdscparecer,
                    to_char(pimdtinclusaomec,'DD/MM/YYYY') as pimdtinclusaomec,
                    usucpfmec,
                    arqidies,
                    pimdscresposta,
                    to_char(pimdtinclusaoies,'DD/MM/YYYY') as pimdtinclusaoies,
                    usucpfies, mntstatus,
                    mtdid, u1.usunome as nome_mec, u2.usunome as nome_ies,
                    u3.usunome as nome_analise,
                    usucpfanaliserec, to_char(pimdtanaliserec,'DD/MM/YYYY') as pimdtanaliserec,
                    pimopcaorecurso, pimdscanaliserec, arqanaliserec
            FROM maismedicomec.parecerinscricaomantida p

            LEFT JOIN seguranca.usuario u1 on u1.usucpf = p.usucpfmec
            LEFT JOIN seguranca.usuario u2 on u2.usucpf = p.usucpfies
            LEFT JOIN seguranca.usuario u3 on u3.usucpf = p.usucpfanaliserec

            WHERE mnmid = {$_REQUEST['mtdid']}
        ";
        $dados = $db->pegaLinha($sql);
    }

    $perfil = pegaPerfilGeral();

    $habilita_mec = 'N';
    if (in_array(PERFIL_MM_MEC_ADMINISTRADOR, $perfil) || in_array(PERFIL_MM_MEC_SUPER_USUARIO, $perfil)) {
        $habilita_mec = 'S';
    }

    $habilita_ies = 'N';
    if (in_array(PERFIL_IES, $perfil) || in_array(PERFIL_MM_MEC_SUPER_USUARIO, $perfil)) {
        $habilita_ies = 'S';
    }
?>

<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>


<script type="text/javascript">

    function validaParecer(tipo){

        if(document.formulario.mtdid.value == ''){
            alert("Informe a Mantida.");
            return false;
        }

        if(tipo == '1'){
            if(document.formulario.pimdscparecer.value == ''){
                alert("Informe o Parecer do MEC.");
                return false;
            }
            if(document.formulario.arqidmec.value == '' && '<?=$dados['arqidmec']?>' == ''){
                alert("Informe o Arquivo MEC.");
                return false;
            }
        }

        if(tipo == '2'){
            if(document.formulario.pimdscresposta.value == ''){
                alert("Informe o Parecer da Mantida.");
                return false;
            }
            if(document.formulario.arqidies.value == '' && '<?=$dados['arqidies']?>' == ''){
                alert("Informe o Arquivo Mantida.");
                return false;
            }
        }

        if(tipo == '3'){
            if(document.formulario.pimdscanaliserec.value == ''){
                alert("Informe o Parecer da Análise.");
                return false;
            }
            if(document.formulario.arqanaliserec.value == '' && '<?=$dados['arqanaliserec']?>' == ''){
                alert("Informe o Arquivo Análise.");
                return false;
            }
        }

        document.formulario.requisicao.value = 'inserir';
        document.formulario.tipo.value = tipo;
        document.formulario.submit();
    }

    function exclusao(url) {
        if (confirm("Deseja excluir este arquivo?")){
            window.location = url;
        }
    }

</script>

<?php
    $mntid = $_SESSION['maismedicomec']['mntid'];
    $sql = "select * from maismedicomec.mantenedora where mntid = ".$mntid;
    $dados_mantenedora = $db->carregar($sql);
?>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
    <tr>
        <td class ="SubTituloDireita" width="35%">Código mantenedora: </td>
        <td><?php echo $mntid ?></td>
    </tr>
    <tr>
        <td class ="SubTituloDireita" width="35%">Razão social</td>
        <td> <?PHP echo $dados_mantenedora[0][mntrazaosocial];?> </td>
    </tr>
</table>

<table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="tabela">
    <tr>
        <td colspan="5" class="subTituloCentro" style="text-align: left; text-transform:uppercase;"> ARQUIVO(S) </td>
    </tr>
    <tr>
        <td>
            <?PHP
                $icone = "<img border=0 src=\"../imagens/check_p.gif\" />";
                $sql = "
                    SELECT  UPPER( m.mtddsc || ' - ' || mundescricao) AS mantida,
                            CASE WHEN p.arqidmec IS NOT NULL
                                    THEN '{$icone}'
                                    ELSE '-'
                            END as arqmec,

                            CASE WHEN p.arqidies IS NOT NULL
                                    THEN '{$icone}'
                                    ELSE '-'
                            END as arqmantida,

                            CASE WHEN p.arqanaliserec IS NOT NULL
                                    THEN '{$icone}'
                                    ELSE '-'
                            END AS arqanalise

                    FROM maismedicomec.mantida AS m

                    JOIN maismedicomec.mantidamunicipio AS mt ON mt.mtdid = m.mtdid
                    JOIN territorios.municipio AS mu ON mu.muncod = mt.muncod
                    LEFT JOIN maismedicomec.parecerinscricaomantida p ON p.mnmid = mt.mnmid
                    LEFT JOIN maismedicomec.arqparecermantida AS arq ON arq.pimid = p.pimid

                    WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}

                    GROUP BY m.mtddsc, mundescricao, arqmec, arqmantida, arqanalise

                    UNION

                    SELECT  UPPER (m.mtddsc || ' - ' || mundescricao) AS mantida,
                            CASE WHEN p.arqidmec IS NOT NULL
                                    THEN '{$icone}'
                                    ELSE '-'
                            END as arqmec,

                            CASE WHEN p.arqidies IS NOT NULL
                                    THEN '{$icone}'
                                    ELSE '-'
                            END as arqmantida,

                            CASE WHEN p.arqanaliserec IS NOT NULL
                                    THEN '{$icone}'
                                    ELSE '-'
                            END AS arqanalise

                    FROM maismedicomec.mantidacandidata AS m
                    JOIN maismedicomec.mantidamunicipiocandidata AS mt ON mt.mtdid = m.mtdid
                    JOIN territorios.municipio AS mu ON mu.muncod = mt.muncod
                    LEFT JOIN maismedicomec.parecerinscricaomantida p ON p.mnmid = mt.mnmid
                    LEFT JOIN maismedicomec.arqparecermantida AS arq ON arq.pimid = p.pimid

                    WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}

                    GROUP BY m.mtddsc, mundescricao, arqmec, arqmantida, arqanalise

                    ORDER BY mantida
                ";
                $cabecalho = array("Mantida","Arquivo MEC","Arquivo Mantida","Arquivo Análise MEC");
                $alinhamento = Array('left','center','center','center');
                $tamanho = Array('','','');
                $db->monta_lista($sql, $cabecalho, 100, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
            ?>
        </td>
    </tr>
</table>

<form name="formulario" id="formulario" method="post" action="" enctype="multipart/form-data">

    <input type="hidden" id="requisicao" name="requisicao" value="">

    <input type="hidden" id="pimid" name="pimid" value="<?=$dados['pimid']?>">

    <input type="hidden" id="tipo" name="tipo" value="">

    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td>
                <table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                    <tr>
                        <td class ="SubTituloDireita" width="15%"> Mantida Indicada/Candidata: </td>
                        <td>
                            <?PHP
                            $mtdid = $dados['mtdid'] ? $dados['mtdid'] : $_REQUEST['mtdid'] ;
                            $sql = "
                                    SELECT  mt.mnmid AS codigo,
                                            m.mtddsc || ' - ' || mundescricao AS descricao
                                    FROM maismedicomec.mantida AS m
                                    JOIN maismedicomec.mantidamunicipio AS mt ON mt.mtdid = m.mtdid
                                    JOIN territorios.municipio AS mu ON mu.muncod = mt.muncod
                                    WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}
                                    group by mt.mnmid, m.mtddsc, mundescricao

                                    union

                                    SELECT  mt.mnmid AS codigo,
                                            m.mtddsc || ' - ' || mundescricao AS descricao
                                    FROM maismedicomec.mantidacandidata AS m
                                    JOIN maismedicomec.mantidamunicipiocandidata AS mt ON mt.mtdid = m.mtdid
                                    JOIN territorios.municipio AS mu ON mu.muncod = mt.muncod
                                    WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}
                                    group by mt.mnmid, m.mtddsc, mundescricao

                                    ORDER BY 2
                                ";
                            $db->monta_combo('mtdid', $sql, 'S', "Selecione...", '', '', '', '', 'S', 'mtdid', '', $mtdid, 'Mantida', "onchange='document.formulario.submit();'");
                            ?>
                        </td>
                    </tr>
                </table>

                <br>

                <table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                    <tr>
                        <td class="subTituloCentro" colspan="2" style="text-align: left; text-transform:uppercase;"> Dados do Responsável pelo Parecer do MEC </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita"> CPF: </td>
                        <td><?= $dados['usucpfmec'] ? $dados['usucpfmec'] : '-'; ?></td>
                    </tr>
                    <tr>
                        <td width="15%" class="SubTituloDireita"> Nome: </td>
                        <td><?= $dados['nome_mec'] ? $dados['nome_mec'] : '-'; ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Data de inserção do Parecer:</td>
                        <td> <?= $dados['pimdtinclusaomec'] ? $dados['pimdtinclusaomec'] : '-'; ?> </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Descrição do Parecer do MEC:</td>
                        <td>
                            <?php
                               echo campo_textarea('pimdscparecer', 'S', $habilita_mec, '', '150', '5', '10000', '', 0, '', false, NULL, $dados['pimdscparecer'], '99%');
                            ?>
                        </td>
                    </tr>
                    <tr>

                    </tr>
                    <tr width="">
                        <td class="subtituloDireita">Arquivo:</td>
                        <td>
                            <input type="file" name="arqidmec" <?if($habilita_mec == 'N'){echo 'disabled';}?> >
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório." >

                            <br>
                            <table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                                <tr>
                                    <td>
                                        <?PHP
                                            if( $dados['pimid'] ){
                                                $pimid = $dados['pimid'];
                                                listagemArquivosParecer( $pimid, $habilita_mec, 'M' );
                                            }
                                        ?>
                                    </td>
                                </tr>
                            </table>

                        </td>
                    </tr>
                    <tr>
                        <td colspan="2" class="SubTituloCentro">
                            <? if($habilita_mec == 'S'){ ?>
                                <input type="button" value="Salvar" id="salvar_p_1" onclick="validaParecer('1');">
                                <input type="button" value="Voltar" id="btnVoltar" onclick="history.back();">
                            <? } ?>
                        </td>
                    </tr>
                </table>

                <table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                    <tr>
                        <td class="subTituloCentro" colspan="2" style="text-align: left; text-transform:uppercase;"> Dados do Responsável pelo Recurso da Mantida </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita"> CPF: </td>
                        <td><?= $dados['usucpfies'] ? $dados['usucpfies'] : '-'; ?></td>
                    </tr>
                    <tr>
                        <td width="15%" class="SubTituloDireita"> Nome: </td>
                        <td><?= $dados['nome_ies'] ? $dados['nome_ies'] : '-'; ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Data de inserção do Recurso:</td>
                        <td> <?= $dadosRecurso['pimdtinclusaoies'] ? $dadosRecurso['pimdtinclusaoies'] : '-'; ?> </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Descrição do Recurso da Mantida:</td>
                        <td>
                            <?php echo campo_textarea('pimdscresposta', 'S', $habilita_ies, '', '150', '5', '10000', '', 0, '', false, NULL, $dados['pimdscresposta'], '99%'); ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="subtituloDireita">Arquivo:</td>
                        <td>
                            <input type="file" id="arqidies" name="arqidies" <?if($habilita_ies == 'N'){echo 'disabled';}?> >
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório." >
                            <br>
                            <table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                                <tr>
                                    <td id="td_lista_arquivos_anexos">
                                        <?PHP
                                            if( $dados['pimid'] ){
                                                $pimid = $dados['pimid'];
                                                listagemArquivosParecer( $pimid, $habilita_ies, 'R' );
                                            }
                                        ?>
                                    </td>
                                </tr>
                            </table>
                        </td>
                    </tr>

                    <?PHP
                        $habilita = buscaLiberacao_Data_Extraordinaria();
                        
                        if( $habilita == 'S' ){ 
                    ?>
                            <tr>
                                <td colspan="2" class="SubTituloCentro">
                                    <?if($habilita_ies == 'S'){ ?>
                                        <?if($dados['arqidmec']){?>
                                            <input type="button" value="Salvar" id="salvar_p_2" onclick="validaParecer('2');">
                                        <?}else{?>
                                            <input type="button" value="Salvar" id="salvar_p_2" onclick="alert('É necessário esperar a inclusão do parecer do MEC!');">
                                        <?}?>
                                        <input type="button" value="Voltar" id="btnVoltar" onclick="history.back();">
                                    <?}?>
                                </td>
                            </tr>
                    <?  }else{ ?>
                            <tr>
                                <td colspan="2" class="SubTituloCentro">
                                    <b style="color:red;">NÃO É POSSÍVEL INSERIR O RECURSO DO MUNICÍPIO, O PRAZO FINAL FOI "05/08/2015"</b>
                                </td>
                            </tr>
                    <?  } ?>
                </table>

                <table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                    <tr>
                        <td class="subTituloCentro" colspan="2" style="text-align: left; text-transform:uppercase;"> Dados do Responsável pela Análise do Parecer do MEC </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita"> CPF: </td>
                        <td><?= $dados['usucpfanaliserec'] ? $dados['usucpfanaliserec'] : '-'; ?></td>
                    </tr>
                    <tr>
                        <td width="15%" class="SubTituloDireita"> Nome: </td>
                        <td><?= $dados['nome_analise'] ? $dados['nome_analise'] : '-'; ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Data de inserção da Análise:</td>
                        <td> <?= $dados['pimdtanaliserec'] ? $dados['pimdtanaliserec'] : '-'; ?> </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Situação:</td>
                        <td>
                            <label><input type="radio" <?=$habilita_mec == 'N' ? 'disabled=disabled' : ''?> name="pimopcaorecurso" value="D" <?=$dados['pimopcaorecurso'] == 't' ? 'checked=checked' : '' ?> > Deferido</label>
                            <label><input type="radio" <?=$habilita_mec == 'N' ? 'disabled=disabled' : ''?> name="pimopcaorecurso" value="I" <?=$dados['pimopcaorecurso'] == 't' ? '' : 'checked=checked' ?>> Indeferido</label>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Descrição da Análise:</td>
                        <td>
                            <?php
                               echo campo_textarea('pimdscanaliserec', 'S', $habilita_mec, '', '150', '5', '10000', '', 0, '', false, NULL, $dados['pimdscanaliserec'], '99%');
                            ?>
                        </td>
                    </tr>
                    <tr width="">
                        <td class="subtituloDireita">Arquivo:</td>
                        <td>
                            <input type="file" name="arqanaliserec" <?if($habilita_mec == 'N'){echo 'disabled';}?> >
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório." >

                            <br>
                            <table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                                <tr>
                                    <td>
                                        <?PHP
                                            if( $dados['pimid'] ){
                                                $pimid = $dados['pimid'];
                                                listagemArquivosParecer( $pimid, $habilita_mec, 'P' );
                                            }
                                        ?>
                                    </td>
                                </tr>
                            </table>

                        </td>
                    </tr>
                    <tr>
                        <td colspan="2" class="SubTituloCentro">

                            <?PHP if($habilita_mec == 'S'){ ?>

                                <input type="button" value="Salvar" id="salvar_p_3" onclick="validaParecer('3');">
                                <input type="button" value="Voltar" id="btnVoltar" onclick="history.back();">

                            <? } ?>
                        </td>
                    </tr>
                </table>

            </td>
    </table>
</form>