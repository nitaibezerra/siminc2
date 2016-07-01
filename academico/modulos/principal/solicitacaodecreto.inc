<?PHP
$_SESSION['academico']['entid'] = $_REQUEST['entidunidade'] ? $_REQUEST['entidunidade'] : $_SESSION['academico']['entid'];
    if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST, $_FILES);
	exit;
    }

    if($_REQUEST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicaoAjax']();
	exit;
    }

    function filtraGND($tpaid = null) {
	global $db;
	$tpaid = !$tpaid ? $_POST['tpaid'] : $tpaid;
	switch($tpaid){
            case "1":
                $arrGND = array(3,4);
                break;
            case "2":
                $arrGND = array(3);
                break;
            case "3":
                $arrGND = array(4);
                break;
            case "4":
                $arrGND = array(4,5);
                break;
            case "5":
                $arrGND = array(3);
                break;
            case "6":
                $arrGND = array(3);
                break;
	}
	if(!$tpaid || !$arrGND){
		$sql = "SELECT gndcod as codigo, gndcod || ' - ' ||  gnddsc as descricao FROM public.gnd where gndstatus = 'A' and 1=2 order by gndcod";
		$permissao = "N";
	}else{
		$sql = "SELECT gndcod as codigo, gndcod || ' - ' ||  gnddsc as descricao FROM public.gnd where gndstatus = 'A' and gndcod in (".implode(",",$arrGND).") order by gndcod";
		$permissao = "S";
	}
	$db->monta_combo('gndcod', $sql, $permissao, 'Selecione', '', '', '', '200', 'S', 'gndcod');
    }

    # - anexarDocumentos: TELA SOLICITAÇÃO DE DECRETO (DECRETO 7689) - ANEXAR DOCUMENTOS.
    function anexarDocumentos($dados, $files, $sldid) {
        global $db;

        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        $sldid          = $sldid;
        $tadid          = 1; #O TIPO DE ARQUIVO, POR ENGUANTO É FIXO EM "1". ATÉ O MOMENTO NÃO NECESSIDADE, EXISTE APENAS O TIPO "1".
        $usucpf         = $_SESSION['usucpf'];
        $aqddtinclusao  = "'".gmdate('Y-m-d')."'";

        $campos = array(
            "sldid"         => $sldid,
            "tadid"         => $tadid,
            "usucpf"        => $usucpf,
            "aqdstatus"     => "'A'",
            "aqddtinclusao" => $aqddtinclusao
        );

        $file = new FilesSimec("arquivodecreto", $campos, "academico");

        if ( $files ) {
            $arquivoSalvo = $file->setUpload("Rede Federal - Solicitação de Descreto");
            if ($arquivoSalvo) {
                return 'S';
            }else{
                return 'N';
            }
        }
        exit;
    }

    # - dowloadDocAnexo: TELA SOLICITAÇÃO DE DECRETO (DECRETO 7689) - ANEXAR DOCUMENTOS.
    function dowloadDocAnexo( $dados ){

        $arqid = $dados['arqid'];

        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        if ( $arqid ){
            $file = new FilesSimec("arquivodecreto", $campos, "academico");
            $file->getDownloadArquivo( $arqid );
        }
    }

    # - excluirDocAnexo: TELA SOLICITAÇÃO DE DECRETO (DECRETO 7689) - ANEXAR DOCUMENTOS.
    function excluirDocAnexo2( $dados ) {
       global $db;

       $arqid = $dados['arqid'];
       $sbsid = $dados['sbsid'];

       include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

       if ($arqid != '') {
           $sql = " UPDATE academico.arquivodecreto SET aqdstatus = 'I' WHERE arqid = {$arqid} ";
       }

       if( $db->executar($sql) ){
           $file = new FilesSimec("arquivodecreto", $campos, "academico");
           $file->excluiArquivoFisico( $arqid );

           $db->commit();
           $db->sucesso('principal/solicitacaodecretonovo', '&acao=A&sbsid='.$sbsid);
       }
    }

    function inserirSolitacaoDecreto($dados, $files) {
	global $db;
	$slfid = pegarMomento();

	if(!$slfid) {
            die("<script>
                    alert('Acesso negado');
                    window.location='academico.php?modulo=principal/solicitacaodecreto&acao=A';
             </script>");
	}

	$sql = "
            INSERT INTO academico.solicitacaodecreto(
                    entid,
                    tpaid,
                    usucpf,
                    sldobjeto,
                    sldjustificativa,
                    sldvlrestimado,
                    sldstatus,
                    slddtinclusao,
                    gndcod, slfid
                ) VALUES (
                    '{$_SESSION['academico']['entid']}',
                    '{$dados['tpaid']}',
                    '{$_SESSION['usucpf']}',
                    '{$dados['sldobjeto']}',
                    '{$dados['sldjustificativa']}',
                    '".str_replace( array(".",","), array("","."), $dados['sldvlrestimado'])."',
                    'A',
                    'NOW()',
                    '{$dados['gndcod']}',
                    '{$slfid}'
            ) RETURNING sldid;
        ";
        $sldid = $db->pegaUm($sql);
        $db->commit();

        if($_FILES['arquivoDecreto']['name'] != ''){
            $result = anexarDocumentos($_REQUEST, $_FILES, $sldid);

            if( $result == 'S' ){
                $db->sucesso('principal/solicitacaodecreto', '&acao=A&decreto=7446', 'Dados inserido e Documentos Anexados com sucesso!');
            }else{
                $db->sucesso('principal/solicitacaodecreto', '&acao=A&decreto=7446', 'Dados inserido com sucesso e Problemas com os Documentos Anexados, tente novamente mais tarde!');
            }

        }else{
            $db->sucesso('principal/solicitacaodecreto', '&acao=A&decreto=7446', 'Dados inserido com sucesso!');
        }
    }

    function atualizarSolitacaoDecreto($dados, $files) {
        global $db;

        $sql = "SELECT * FROM academico.solicitacaodecreto WHERE sldid='".$dados['sldid']."'";
        $solicitacaodecreto = $db->pegaLinha($sql);
        $slfid = $solicitacaodecreto['slfid'];
        $slfid_ = pegarMomento();

        if($slfid != $slfid_) {
            die("<script>
                    alert('Acesso negado');
                    window.location='academico.php?modulo=principal/solicitacaodecreto&acao=A';
             </script>");
        }

        $sql = "
            UPDATE academico.solicitacaodecreto
                    SET tpaid               ='".$dados['tpaid']."',
                        gndcod              ='".$dados['gndcod']."',
                        sldobjeto           ='".$dados['sldobjeto']."',
                        sldjustificativa    ='".$dados['sldjustificativa']."',
                        sldvlrestimado      ='".str_replace(array(".",","),array("","."),$dados['sldvlrestimado'])."'
                WHERE sldid = '".$dados['sldid']."' RETURNING sldid;
        ";
        $sldid = $db->pegaUm($sql);
        $db->commit();

        if($_FILES['arquivoDecreto']['name'] != ''){
            $result = anexarDocumentos($_REQUEST, $_FILES, $sldid);

            if( $result == 'S' ){
                $db->sucesso('principal/solicitacaodecreto', '&acao=A&decreto=7446&sldid='.$sldid, 'Dados Atualizados e Documentos Anexados com sucesso!');
            }else{
                $db->sucesso('principal/solicitacaodecreto', '&acao=A&decreto=7446&sldid='.$sldid, 'Dados Atualizados com sucesso e Problemas com os Documentos Anexados, tente novamente mais tarde!');
            }

        }else{
            $db->sucesso('principal/solicitacaodecreto', '&acao=A&decreto=7446&sldid='.$sldid, 'Dados Atualizados com sucesso!');
        }
    }

    function pegarMomento() {
	global $db;
	$sql = "SELECT slfid FROM academico.sldfase WHERE slfdatainicio<='".date("Y-m-d")."' AND slfdatafim>='".date("Y-m-d")."'";
	$slfid = $db->pegaUm($sql);
	return $slfid;
    }

    function excluirSolicitacaoDecreto($dados) {
	global $db;

	$sql = "SELECT * FROM academico.solicitacaodecreto WHERE sldid='".$dados['sldid']."'";
	$solicitacaodecreto = $db->pegaLinha($sql);
	$slfid = $solicitacaodecreto['slfid'];
	$slfid_ = pegarMomento();

	if($slfid != $slfid_) {
            die("<script>
                    alert('Acesso negado');
                    window.location='academico.php?modulo=principal/solicitacaodecreto&acao=A';
             </script>");
	}

	$sql = "
            UPDATE academico.solicitacaodecreto
                SET sldstatus = 'I'
            WHERE sldid='".$dados['sldid']."'
        ";
        $db->executar($sql);
        $db->commit();
        echo "<script>alert('Removido com sucesso');window.location='academico.php?modulo=principal/solicitacaodecreto&acao=A';</script>";
    }

    function permissaoAlterar() {
        global $db;

        $arrPerfil = pegaPerfilGeral();

        if( in_array(PERFIL_SUPERUSUARIO,$arrPerfil)  ||
                in_array(PERFIL_ADMINISTRADOR,$arrPerfil) ||
                in_array(PERFIL_REITOR,$arrPerfil)        ||
                in_array(PERFIL_IFESCADBOLSAS,$arrPerfil) ||
                in_array(PERFIL_IFESCADCURSOS,$arrPerfil) ||
                in_array(PERFIL_IFESCADASTRO,$arrPerfil)  ||
                in_array(PERFIL_CADASTROGERAL,$arrPerfil) ||
                in_array(PERFIL_PROREITOR,$arrPerfil) ||
                in_array(PERFIL_INTERLOCUTOR_INSTITUTO,$arrPerfil)
        ){
            return true;
        }else{
            return false;
        }
    }

    function listaJustificativas(){
	global $db;

	$sql = "
            SELECT  slfdsc,
                    sljdsc,
                    to_char(sljdataatualizacao, 'DD/MM/YYYY')

            FROM academico.sldfasejustificativa slj

            INNER JOIN academico.sldfase slf ON slf.slfid = slj.slfid

            WHERE sljstatus = 'A'

            ORDER BY slf.slfid
        ";
	if($permissoes){
            $cabecalho = array("Ações", "Momento", "Justificativa", "Ultima atualização");
	}else{
            $cabecalho = array("Momento", "Justificativa", "Ultima atualização");
	}
	return $db->monta_lista($sql, $cabecalho, 50, 20, '', '100%', '',$arrayDeTiposParaOrdenacao);
    }

    $_SESSION['academico']['entid'] = $_REQUEST['entid'] ? $_REQUEST['entid'] : $_SESSION['academico']['entid'];
    $_SESSION['academico']['entidadenivel'] = 'unidade';

    $perfilNotBloq = array(
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

    //Verificando a segurança
    $permissoes = verificaPerfilAcademico($perfilNotBloq);
    validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['academico']['orgid']);
    // Fim segurança

    if(!$_SESSION['academico']['entid']) {
	die("<script>
                alert('Entidade não encontrada. Navegue novamente.');
		window.location='academico.php?modulo=inicio&acao=C';
            </script>");
    }

    require_once APPRAIZ . "includes/cabecalho.inc";
    echo '<br/>';

    $abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57334' : '57335';

    #$db->cria_aba($abacod_tela,$url,$parametros);

?>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

<script>
    function validarFormulario() {
        if(document.getElementById('tpaid').value=='') {
            alert('Tipo de solicitação obrigatório');
            return false;
        }
        if(document.getElementById('sldobjeto').value=='') {
            alert('Objeto obrigatório');
            return false;
        }
        if(document.getElementById('sldjustificativa').value=='') {
            alert('Justificativa obrigatório');
            return false;
        }
        if(document.getElementById('gndcod').value=='') {
            alert('GND obrigatório');
            return false;
        }
        if(document.getElementById('sldvlrestimado').value=='') {
            alert('Valor estimado obrigatório');
            return false;
        }

        document.getElementById('sldvlrestimado').onkeyup();
        document.getElementById('formulario').submit();
    }

    function excluir(sldid) {
        var conf = confirm('Deseja realmente excluir?');
        if(conf) {
            document.getElementById('requisicao').value="excluirSolicitacaoDecreto";
            document.getElementById('sldid').value=sldid;
            document.getElementById('formulario').submit();
        }
    }

    function dowloadDocAnexo( arqid ){
        $('#arqid').val(arqid);
        $('#requisicao').val('dowloadDocAnexo');
        $('#formulario').submit();
    }

    function excluirDocAnexo(arqid){
        var confirma = confirm("Deseja realmente excluir o Arquivo?");
        if( confirma ){
            $('#arqid').val(arqid);
            $('#requisicao').val('excluirDocAnexo2');
            $('#formulario').submit();
        }
    }

    function filtraGND(tpaid){
        $.ajax({
            type: "POST",
            url: window.location,
            data: "requisicaoAjax=filtraGND&tpaid=" + tpaid,
            success: function(msg){
                $('#td_gnd').html( msg );
            }
        });
    }

    function selecionarDecreto(decreto){
        if(decreto == 7446)
            window.location = 'academico.php?modulo=principal/solicitacaodecreto&acao=A&decreto=7446';
        else if(decreto == 7689)
            window.location = 'academico.php?modulo=principal/solicitacaodecretonovo&acao=A';
        else
            window.location = 'academico.php?modulo=principal/solicitacaodecreto&acao=A';
    }

</script>
<?php
if($_REQUEST['decreto'] == ''){
    monta_titulo('Solicitação Decreto','');
        
}elseif($_REQUEST['decreto'] == '7446'){
    monta_titulo('Solicitação de Autorização - Decreto 7.446/2011','');
}
?>

<div class='col-md-2' style='position:static;'>
    <?=cria_aba_2($abacod_tela, $url, $parametros);?>
</div>
<div class='col-md-9' style='position:static;'>
<?php
    $tableForm = '
        <form id="formulario" name="formulario" method="post" action="academico.php?modulo=principal/solicitacaodecreto&acao=A&decreto=7446" enctype="multipart/form-data">
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    ';

    $selected = '';
    if($_REQUEST['decreto'] == '7446')
        $selected = 'selected';

    $comboDecreto = '
        <tr>
            <td width="35%" class="SubTituloDireita">Decreto:</td>
            <td>
                <select name="tipo_decreto" onChange="selecionarDecreto(this.value)" class="CampoEstilo obrigatorio" style="width:300px">
                    <option value="">Selecione</option>
                    <option value="7446" ' . $selected . '>Solicitação de Autorização - Decreto 7.446/2011</option>
                    <option value="7689">Solicitação de Autorização - Decreto 7.689/2012</option>
                </select>
            </td>
        </tr>
    ';

    if($_REQUEST['decreto'] == ''){
        // cria o cabeçalho padrão do módulo
        $autoriazacaoconcursos = new autoriazacaoconcursos();
        echo $autoriazacaoconcursos->cabecalho_entidade($_SESSION['academico']['entid']);
        #academico_monta_cabecalho_sig($_SESSION['academico']['entid'], 'Solicitação Decreto');
        echo $tableForm;
        echo $comboDecreto;
        echo '</table></form>';

    }elseif($_REQUEST['decreto'] == '7446'){

        #academico_monta_cabecalho_sig($_SESSION['academico']['entid'], 'Solicitação de Autorização - Decreto 7.446/2011');
        // cria o cabeçalho padrão do módulo
        $autoriazacaoconcursos = new autoriazacaoconcursos();
        echo $autoriazacaoconcursos->cabecalho_entidade($_SESSION['academico']['entid']);
        $permissaoAlterar = permissaoAlterar();
        $req = "inserirSolitacaoDecreto";

        if($_REQUEST['sldid']) {
            $sql = "SELECT * FROM academico.solicitacaodecreto WHERE sldid='".$_REQUEST['sldid']."'";
            $solicitacaodecreto = $db->pegaLinha($sql);
            $slfid_ = pegarMomento();
            $slfid = $solicitacaodecreto['slfid'];

            if($slfid != $slfid_) {
                die("<script>
                        alert('Acesso negado');
                        window.location='academico.php?modulo=principal/solicitacaodecreto&acao=A';
                 </script>");
            }

            $sldid = $solicitacaodecreto['sldid'];
            $tpaid = $solicitacaodecreto['tpaid'];
            $gndcod = $solicitacaodecreto['gndcod'];
            $sldobjeto = $solicitacaodecreto['sldobjeto'];
            $sldjustificativa = $solicitacaodecreto['sldjustificativa'];
            $sldvlrestimado = number_format($solicitacaodecreto['sldvlrestimado'],2,",",".");
            $req = "atualizarSolitacaoDecreto";
        }

        if($permissaoAlterar){
            echo $tableForm;
?>
            <input type="hidden" name="requisicao" id="requisicao" value="<? echo $req; ?>">
            <input type="hidden" name="sldid" id="sldid" value="<? echo $sldid; ?>">

            <input type="hidden" id="salvarComAnexo" name="salvarComAnexo" value="">
            <input type="hidden" id="arqid" name="arqid" value="">

            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">

                <?php echo $comboDecreto; ?>

                <tr>
                    <td class="SubTituloDireita">Tipo:</td>
                    <td>
                        <?PHP
                            $sql = "SELECT tpaid as codigo, tpadsc as descricao FROM academico.tiposolicitacao";
                            $db->monta_combo('tpaid', $sql, 'S', 'Selecione', 'filtraGND', '', '', '200', 'S', 'tpaid');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Objeto:</td>
                    <td>
                        <?PHP
                            echo campo_texto('sldobjeto', "S", "S", "Objeto", 67, 150, "", "", '', '', 0, 'id="sldobjeto"');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Justificativa:</td>
                    <td>
                        <?PHP
                            echo campo_textarea('sldjustificativa', 'S', 'S', '', '70', '4', '400');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">GND:</td>
                    <td id="td_gnd" >
                        <?PHP
                            if ($tpaid) {
                                filtraGND($tpaid);
                            } else {
                                $sql = "SELECT gndcod as codigo, gndcod || ' - ' ||  gnddsc as descricao FROM public.gnd where gndstatus = 'A' and gndcod in (3,4) order by gndcod";
                                $db->monta_combo('gndcod', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'gndcod');
                            }
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Valor:</td>
                    <td>
                        <?PHP
                            echo campo_texto('sldvlrestimado', "S", "S", "Valor", 30, 14, "###.###.###,##", "", '', '', 0, 'id="sldvlrestimado"');
                        ?>
                    </td>
                </tr>

                <tr>
                    <td class="SubTituloDireita" width="25%">Arquivo:</td>
                    <td colspan="2">
                        <input type="file" name="arquivoDecreto" id="arquivoDecreto">
                    </td>
                </tr>
                <?PHP
                    if( $sldid != '' ){
                ?>
                    <tr>
                        <td class="SubTituloDireita" width="25%">Lista de Arquivos:</td>
                        <td colspan="2">
                            <?PHP
                                $acao = "
                                    <img border=\"0\" onclick=\"excluirDocAnexo('|| arq.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/excluir.gif\" />
                                    <img border=\"0\" onclick=\"dowloadDocAnexo('|| arq.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/indicador-vermelha.png\" />
                                ";

                                $desativado = "
                                    <img border=\"0\" align=\"absmiddle\" src=\"../imagens/excluir_01.gif\" />
                                    <img border=\"0\" onclick=\"dowloadDocAnexo('|| arq.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/indicador-vermelha.png\" />
                                ";

                                $down = "<a title=\"Download\" href=\"#\" onclick=\"dowloadDocAnexo('|| arq.arqid ||');\">' || arq.arqnome || '</a>";

                                $sql = "
                                    SELECT  CASE WHEN anx.usucpf = '{$_SESSION['usucpf']}'
                                                THEN '{$acao}'
                                                ELSE '{$desativado}'
                                            END as acao,
                                            '{$down}' as descricao,
                                            td.taddsc as tipo_arquivo_decreto,
                                            arq.arqnome||'.'||arq.arqextensao,
                                            to_char(aqddtinclusao, 'DD/MM/YYYY') as aqddtinclusao
                                    FROM academico.arquivodecreto anx
                                    JOIN academico.tipoarquivodecreto td on td.tadid = anx.tadid
                                    JOIN public.arquivo arq on arq.arqid = anx.arqid
                                    WHERE anx.aqdstatus = 'A' AND sldid = {$sldid}
                                ";
                                $cabecalho = Array("Ação", "Descrição",  "Tipo de Documento", "Nome do arquivo", "Data da Inclusão");
                                //$whidth = Array('20%', '60%', '20%');
                                $align  = Array('center', 'left', 'left', 'left', 'right');
                                $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $whidth, $align, '');

                            ?>
                        </td>
                    </tr>
                <?PHP } ?>

                <tr>
                    <td align="center" colspan="2"><input type="button" name="salvar" value="Salvar" onclick="validarFormulario();"> <input type="button" name="novo" value="Novo" onclick="window.location='academico.php?modulo=principal/solicitacaodecreto&acao=A&decreto=7446';"></td>
                </tr>
            </table>
        </form>

<?php   }   ?>

<div id="lista">  <?=listaJustificativas() ?> </div>

<?PHP
        $slfid = pegarMomento();

        if(!$slfid) {
                $permissaoAlterar = false;
        }

        $sql = "
            SELECT  ".($permissaoAlterar ? " CASE WHEN s.slfid=".$slfid." THEN '<span style=\"white-space:nowrap;\" ><img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"window.location=\'academico.php?modulo=principal/solicitacaodecreto&acao=A&decreto=7446&sldid='||s.sldid||'\';\"> <img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluir(\''||s.sldid||'\');\"></span>' ELSE '&nbsp;' END as acao," : "")."
                    s.sldid,
                    sl.slfdsc,
                    e.entnome, tpadsc, sldobjeto, sldjustificativa, g.gndcod || ' - ' || COALESCE(gnddsc,'N/A') AS gnddsc, sldvlrestimado, to_char(slddtinclusao, 'dd/mm/YYYY HH24:MI') as slddtinclusao

            FROM academico.solicitacaodecreto s

            LEFT JOIN entidade.entidade e ON e.entid = s.entid
            LEFT JOIN academico.tiposolicitacao t ON t.tpaid = s.tpaid
            LEFT JOIN public.gnd g ON g.gndcod = s.gndcod
            INNER JOIN academico.sldfase sl ON sl.slfid = s.slfid
            WHERE sldstatus='A' AND s.entid='".$_SESSION['academico']['entid']."'
            ORDER BY sldid
        ";

        if($permissaoAlterar){
            $cabecalho = array("&nbsp","Cód.","Momento","Nome da instituição","Tipo Solicitação","Objeto","Justificativa","GND","Valor Estimado","Data Inclusão");
        }else{
            $cabecalho = array("Cód.","Momento","Nome da instituição","Tipo Solicitação","Objeto","Justificativa","GND","Valor Estimado","Data Inclusão");
        }
        $db->monta_lista_simples($sql,$cabecalho,1000,5,'N','95%',$par2);
    }#end elseif($_REQUEST['decreto'] == '7446')
?>
</div>