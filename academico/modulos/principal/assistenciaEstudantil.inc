<?php

if ($_SESSION['baselogin'] == 'simec_desenvolvimento'){
	define("QUEID_ACADEMICO_INDICADORES", 		76);
	define("QUEID_ACADEMICO_INFRAESTRUTURA", 	77);
	define("QUEID_ACADEMICO_SOCIOECONOMICO", 	81);
}else{
	define("QUEID_ACADEMICO_INDICADORES", 		81);
	define("QUEID_ACADEMICO_INFRAESTRUTURA", 	82);
	define("QUEID_ACADEMICO_SOCIOECONOMICO", 	83);
}
if ($_SESSION['baselogin'] == 'simec_desenvolvimento'){
	define("TPDID_ACADEMICO", 88);
}else{
	define("TPDID_ACADEMICO", 89);
}

if( $_REQUEST['aetdid'] != '' ){
	excluiPeriodo( $_REQUEST['aetdid'] );
}

$perfilNotBloq = array(PERFIL_ASSISTENCIA_ESTUDANTIL_IES, PERFIL_ASSISTENCIA_ESTUDANTIL, PERFIL_ADMINISTRADOR);
// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);
validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['academico']['orgid']);
//Fim Segurança

include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/Montatabela.class.inc";
include APPRAIZ ."includes/workflow.php";

if( $_POST['salvar_questionario'] && $_POST['idTabela'] ){
	$obMonta = new Montatabela();
	$obMonta->salvar();
}

function criarDocumentoAssistencia($entid) {
	global $db;

    $sqlDescricao = "SELECT	entnome FROM entidade.entidade WHERE entid = '".$entid."'";
    $descricao = $db->pegaUm( $sqlDescricao );
    $docdsc = "Cadastramento Rede Federal - " . $descricao;

    $docid = wf_cadastrarDocumento( TPDID_ACADEMICO, $docdsc );
    return $docid;
}

function excluiPeriodo($aetdid){
    global $db;

    //$sql = "DELETE FROM academico.assistenciaestudantil where aetdid = {$aetdid}";
    $sql = " UPDATE academico.assistenciaestudantil SET aetstatus = 'I' WHERE aetdid = {$aetdid} ";
    $db->executar($sql);
    $db->commit();

    exit;
}


function salvarQuestionario($entid, $ano, $semestre){
	global $db;

	//include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";

	$sql = "select aetdid from academico.assistenciaestudantil where aetano = {$ano} and aetsemestre={$semestre} and entid={$entid}";

	$result = $db->pegaLinha( $sql );

	if (!$result){
		$sql = "SELECT
		entnome
		FROM
		entidade.entidade
		WHERE
		entid = {$entid}";

	    $titulo = $db->pegaUm( $sql );

	    // insere os questionarios de indicadores, infraestrutura e socioeconomico

	    $arParam = array ( "queid" => QUEID_ACADEMICO_INDICADORES, "titulo" => "Acadêmico (".$titulo.")" );
		$qrpid = GerenciaQuestionario::insereQuestionario( $arParam );

		$docid = criarDocumentoAssistencia($entid);

	    $sql = "INSERT INTO academico.assistenciaestudantil (usucpf,entid,  aetano, aetsemestre, docid) VALUES ('{$_SESSION['usucpf']}',  {$entid}, {$ano}, {$semestre}, {$docid}) returning aetdid";

	    $aetdid = $db->pegaUm( $sql );
	    $db->commit();

	    $sql = "INSERT INTO academico.itemassistencia values ({$qrpid}, {$aetdid})";
	    $db->executar($sql);
	    $db->commit();

	    //---------

	    $arParam = array ( "queid" => QUEID_ACADEMICO_INFRAESTRUTURA, "titulo" => "Acadêmico (".$titulo.")" );
	    $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );

	    $sql = "INSERT INTO academico.itemassistencia values ({$qrpid}, {$aetdid})";
	    $db->executar($sql);
	    $db->commit();


	    //---------

	    $arParam = array ( "queid" => QUEID_ACADEMICO_SOCIOECONOMICO, "titulo" => "Acadêmico (".$titulo.")" );
	    $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );

	    $sql = "INSERT INTO academico.itemassistencia values ({$qrpid}, {$aetdid})";
	    $db->executar($sql);
	    $db->commit();

	    return $docid;
	}
}


function pegaQrpidIndicadores( $entid, $queid, $ano, $semestre ){
	global $db;
    global $docid;
    //include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";

    $sql = "SELECT
            	it.qrpid, aet.docid
            FROM
            	academico.assistenciaestudantil aet
            INNER JOIN academico.itemassistencia it on it.aetdid = aet.aetdid
			INNER JOIN
            	questionario.questionarioresposta qr ON qr.qrpid = it.qrpid
            WHERE
            	aet.entid = {$entid}
            AND
            	aetano = {$ano}
            AND
            	aetsemestre = {$semestre}
            AND
            	qr.queid = {$queid}";
    //dbg($sql);
    $registro = $db->pegaLinha( $sql );

    if( !$registro ){
    	unset($docid);
    	return  -1;
    }
    else {
    	$docid = $registro['docid'];
    	return $registro['qrpid'];
    }
}

//Chamada de programa
include_once APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";

$abacod_tela = '57136';
#$db->cria_aba($abacod_tela,$url,$parametros);

$linha1 = 'Assistência  Estudantil';
$linha2 = '';

monta_titulo($linha1, $linha2);

$_SESSION['academico']['entidcampus'] = $_REQUEST['entid'] ? $_REQUEST['entid'] : $_SESSION['academico']['entidcampus'];
$entidcampus = $_SESSION['academico']['entidcampus'] ? $_SESSION['academico']['entidcampus'] : $_REQUEST['entid'];
?>
    <br/>
    <div class='col-md-2' style='position:static;'>
        <?=cria_aba_2($abacod_tela, $url, $parametros);?>
    </div>
    <div class='col-md-9' style='position:static;'>
    <?
    // cria o cabeçalho padrão do módulo
    if($entidcampus){
        $autoriazacaoconcursos = new autoriazacaoconcursos();
        echo $autoriazacaoconcursos->cabecalho_entidade($entidcampus);
    }
        ?>
        <div id="valores_show"></div>
    <?
    $entid 		= $_SESSION['academico']['entidcampus'];
    $ano 		= $_REQUEST['aetano'];
    $semestre 	= $_REQUEST['aetsemestre'];

    $novaAssistencia = $_REQUEST['novaAssistencia'];


    if($semestre && $ano && $entid){
        if ($novaAssistencia){
            $docid = salvarQuestionario($entid, $ano, $semestre);
        }

    ?>
        <table cellspacing="0" cellpadding="0" align="center" style="width:95%;">
            <tr>
                <td>
                    <table cellspacing="0" cellpadding="0" align="left">
                        <tbody>
						<?php
						if($_REQUEST['questionario'] == 3){
							$qrpid = pegaQrpidIndicadores($entid, QUEID_ACADEMICO_SOCIOECONOMICO, $ano, $semestre);
							if ($qrpid != -1){
                        ?>
                            <tr>
                                <td valign="top" height="20">
                                    <img border="0" width="11" height="20" alt="" src="../imagens/aba_esq_sel_ini.gif">
                                </td>
                                <td background="../imagens/aba_fundo_sel.gif" align="center" valign="middle" height="20" style="color:#000055; padding-left: 10px; padding-right: 10px;">
                                    Perfil Socioeconômico
                                </td>
                                <td valign="top" height="20">
                                    <img border="0" width="11" height="20" alt="" src="../imagens/aba_dir_sel.gif">
                                </td>
                                <td background="../imagens/aba_fundo_nosel.gif" align="center" valign="middle" height="20" style="color:#4488cc; padding-left: 10px; padding-right: 10px;">
                                    <a title="Informações Gerenciais" style="color:#4488cc;" href="academico.php?modulo=principal/assistenciaEstudantil&acao=A&aetsemestre=<?php echo $semestre; ?>&aetano=<?php echo $ano; ?>&questionario=2">
                                        Infraestrutura
                                    </a>
                                </td>
                                <td valign="top" height="20">
                                    <img border="0" width="11" height="20" alt="" src="../imagens/aba_nosel.gif">
                                </td>
                                <td background="../imagens/aba_fundo_nosel.gif" align="center" valign="middle" height="20" style="color:#4488cc; padding-left: 10px; padding-right: 10px;">
                                    <a title="Informações Gerenciais" style="color:#4488cc;" href="academico.php?modulo=principal/assistenciaEstudantil&acao=A&aetsemestre=<?php echo $semestre; ?>&aetano=<?php echo $ano; ?>&questionario=1">
                                        Atendimentos
                                    </a>
                                </td>
                                <td valign="top" height="20">
                                    <img border="0" width="11" height="20" alt="" src="../imagens/aba_nosel_fim.gif">
                                </td>
                            </tr>
							<?php
							}
						}
						elseif($_REQUEST['questionario'] == 2) {
							$qrpid = pegaQrpidIndicadores($entid, QUEID_ACADEMICO_INFRAESTRUTURA, $ano, $semestre);
							if ($qrpid != -1) {
							?>
                            <tr>
                                <td valign="top" height="20">
                                    <img border="0" width="11" height="20" alt="" src="../imagens/aba_nosel_ini.gif">
                                </td>
                                <td background="../imagens/aba_fundo_nosel.gif" align="center" valign="middle" height="20" style="color:#4488cc; padding-left: 10px; padding-right: 10px;">
                                    <a title="Perfil Socioeconômico" style="color:#4488cc;" href="academico.php?modulo=principal/assistenciaEstudantil&acao=A&aetsemestre=<?php echo $semestre; ?>&aetano=<?php echo $ano; ?>&questionario=3">
                                        Perfil Socioeconômico
                                    </a>
                                </td>
                                <td valign="top" height="20">
                                    <img border="0" width="11" height="20" alt="" src="../imagens/aba_esq_sel.gif">
                                </td>
                                <td background="../imagens/aba_fundo_sel.gif" align="center" valign="middle" height="20" style="color:#000055; padding-left: 10px; padding-right: 10px;">
                                    Infraestrutura
                                </td>
                                <td valign="top" height="20">
                                    <img border="0" width="11" height="20" alt="" src="../imagens/aba_dir_sel.gif">
                                </td>
                                <td background="../imagens/aba_fundo_nosel.gif" align="center" valign="middle" height="20" style="color:#4488cc; padding-left: 10px; padding-right: 10px;">
                                    <a title="Informações Gerenciais" style="color:#4488cc;" href="academico.php?modulo=principal/assistenciaEstudantil&acao=A&aetsemestre=<?php echo $semestre; ?>&aetano=<?php echo $ano; ?>&questionario=1">
                                        Atendimentos
                                    </a>
                                </td>
                                <td valign="top" height="20">
                                    <img border="0" width="11" height="20" alt="" src="../imagens/aba_nosel_fim.gif">
                                </td>
                            </tr>
							<?php
							}
						}else {
							$qrpid = pegaQrpidIndicadores($entid, QUEID_ACADEMICO_INDICADORES, $ano, $semestre);
							if ($qrpid != -1){
							?>
                            <tr>
                                <td valign="top" height="20">
                                    <img border="0" width="11" height="20" alt="" src="../imagens/aba_nosel_ini.gif">
                                </td>
                                <td background="../imagens/aba_fundo_nosel.gif" align="center" valign="middle" height="20" style="color:#4488cc; padding-left: 10px; padding-right: 10px;">
                                    <a title="Perfil Socioeconômico" style="color:#4488cc;" href="academico.php?modulo=principal/assistenciaEstudantil&acao=A&aetsemestre=<?php echo $semestre; ?>&aetano=<?php echo $ano; ?>&questionario=3">
                                        Perfil Socioeconômico
                                    </a>
                                </td>
                                <td valign="top" height="20">
                                    <img border="0" width="11" height="20" alt="" src="../imagens/aba_nosel.gif">
                                </td>
                                <td background="../imagens/aba_fundo_nosel.gif" align="center" valign="middle" height="20" style="color:#4488cc; padding-left: 10px; padding-right: 10px;">
                                    <a title="Informações Gerenciais" style="color:#4488cc;" href="academico.php?modulo=principal/assistenciaEstudantil&acao=A&aetsemestre=<?php echo $semestre; ?>&aetano=<?php echo $ano; ?>&questionario=2">
                                        Infraestrutura
                                    </a>
                                </td>
                                <td valign="top" height="20">
                                    <img border="0" width="11" height="20" alt="" src="../imagens/aba_esq_sel.gif">
                                </td>
                                <td background="../imagens/aba_fundo_sel.gif" align="center" valign="middle" height="20" style="color:#000055; padding-left: 10px; padding-right: 10px;">
                                    Atendimentos
                                </td>
                                <td valign="top" height="20">
                                    <img border="0" width="11" height="20" alt="" src="../imagens/aba_dir_sel.gif">
                                </td>
                            </tr>
						<?php
							}
						} ?>
                        </tbody>
                    </table>
                </td>
            </tr>
        </table>
        <?
        if ($qrpid != -1){
        ?>
        <table width = "100%">
            <tr>
                <td width="90%">
                <?php
                #REGRAS DE ACESSO AO QUESTIONARIO - DE ACORDO COM O PERFIL E ESTADO DO DOCUMENTO.
                $perfis = pegaPerfilGeral($_SESSION['usucpf']);

                $estadoAtual = prePegarEstadoAtual($docid);

                if( in_array(PERFIL_SUPERUSUARIO, $perfis) || in_array(PERFIL_ADMINISTRADOR, $perfis) ){
                    $habilita = 'S';
                }elseif( ( $estadoAtual == WF_ASSISTENCIA_ESTUDANTIL_EM_PREENCHIMENTO || $estadoAtual == WF_ASSISTENCIA_ESTUDANTIL_DEVOLVIDO_PARA_AJUSTE ) && in_array(PERFIL_ASSISTENCIA_ESTUDANTIL_IES, $perfis) ){
                    $habilita = 'S';
                }elseif( ( $estadoAtual == WF_ASSISTENCIA_ESTUDANTIL_EM_ANALISE_MEC ) && in_array(PERFIL_ASSISTENCIA_ESTUDANTIL, $perfis) ){
                    $habilita = 'S';
                }elseif( ( $estadoAtual == WF_ASSISTENCIA_ESTUDANTIL_APROVADO ) ){
                    $habilita = 'N';
                }else{
                    $habilita = 'N';
                }

                new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'habilitado' => $habilita, 'relatorio' => 'modulo=relatorio/relatorioAssistenciaEstudantil&acao=A') );
                ?>
                </td>
                <td valign="top">
                <?
                if ($docid != ''){
                    // Cria o novo objeto de conexão
                    $db = new cls_banco();
                    //wf_pegarEstadoAtual( $docid );
                    wf_desenhaBarraNavegacao( $docid, array());
                }
                ?>
                </td>
            </tr>
        </table>
<?
        }
    }

    if( !$_SESSION['academico']['entid'] ){
        $_SESSION['academico']['entid'] = $_REQUEST['entid'];
    }

    if( $_SESSION['academico']['entid'] != '' ){
        $sql = "select entunicod from entidade.entidade where entid = {$_SESSION['academico']['entid']}";
        $enunicod = $db->pegaUm($sql);
    }else{
        $db->insucesso('Não foi possivél acessar o módulo, tente novamente mais tarde!', '', 'inicio', '&acao=C');
    }

?>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script>
        $(function(){
            $('[name=aetsemestre], [name=aetano]').live('change',function(){

                var aetano = $('[name=aetano]').val();
                var aetsemestre = $('[name=aetsemestre]').val();

                if(aetsemestre != '' && aetano != ''){
                    document.location.href = 'academico.php?modulo=principal/assistenciaEstudantil&acao=A&aetsemestre='+aetsemestre+'&aetano='+aetano;
                }
            });
            $('#valores_show').html($('#valores_temp').html());
            $('#valores_temp').remove();

        });


        function salvar(){

            var aetano = $('[name=aetano]').val();
            var aetsemestre = $('[name=aetsemestre]').val();

            document.location.href = 'academico.php?modulo=principal/assistenciaEstudantil&acao=A&aetsemestre='+aetsemestre+'&novaAssistencia=1&aetano='+aetano;
        }

        function excluiPeriodo(  aetdid ){
            var confirma = confirm("Deseja realmente excluir o Registro?");

            if( confirma ){
                var url = location.href;
                $.ajax({
                    url : url,
                    type: 'post',
                    data: {
                        ajax    : 'excluiPeriodo',
                        aetdid  : aetdid},
                        dataType: "html",
                        async	: false,
                        beforeSend : function (){
                            divCarregando();
                        },
                        error : function (){
                            divCarregado();
                        },
                        success : function ( data ){
                          divCarregado();
                          alert('Período excluido com sucesso');
                          document.location.href = 'academico.php?modulo=principal/assistenciaEstudantil&acao=A';
                    }
                });
                }
        }
    </script>

    <div id="valores_temp" style="display:none;">
        <table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
            <tr>
                <td colspan="2" class="subtituloCentro">Informações SIAFI</td>
            </tr>
            <tr>
                <td class="subtituloDireita" >Dotação Inicial</td>
                <td>
                <?php
                $dotacaoinicial = $dotacaoinicial ? formata_valor($dotacaoinicial) : '';
                echo campo_texto('dotacaoinicial', 'N', 'N', '', 30, 255, '', '');
                ?></td>
            </tr>
            <tr>
                <td class="subtituloDireita" >Dotação Autorizada</td>
                <td>
                <?php
                $dotacaoautorizada = $dotacaoautorizada ? formata_valor($dotacaoautorizada) : '';
                echo campo_texto('dotacaoautorizada', 'N', 'N', '', 30, 255, '', '');
                ?></td>
            </tr>
            <tr>
                <td class="subtituloDireita" >Crédito Disponível</td>
                <td>
                <?php
                $creditodisponivel = $creditodisponivel ? formata_valor($creditodisponivel) : '';
                echo campo_texto('creditodisponivel', 'N', 'N', '', 30, 255, '', '');
                ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita" >Valor Empenhado</td>
                <td>
                <?php
                $empenhado = $empenhado ? formata_valor($empenhado) : '';
                echo campo_texto('empenhado', 'N', 'N', '', 30, 255, '', '');
                ?></td>
            </tr>
            <tr>
                <td class="subtituloDireita" >Valor Executado</td>
                <td>
                <?php
                $executado = $executado ? formata_valor($executado) : '';
                echo campo_texto('executado', 'N', 'N', '', 30, 255, '', '');
                ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita" >Valores Pagos</td>
                <td>
                <?php
                $pago = $pago ? formata_valor($pago) : '';
                echo campo_texto('pago', 'N', 'N', '', 30, 255, '', '');
                ?>
                </td>
            </tr>
            <tr>
                <td colspan="2" class="subtituloEsquerda">
                <table width="100%">
                <tr>
                <td>
                    <?php

                    $sql = "select distinct
                                        ";
                    if ( !verificaPerfil( array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR) ) ){

                            $sql.="'
                                    <center>
                                        <a href=\"academico.php?modulo=principal/assistenciaEstudantil&acao=A&aetano='||aetano||'&aetsemestre='||aetsemestre||'&evento=A\">
                                            <img src=\"../imagens/alterar.gif\" border=0 title=\"Visualizar\">
                                        </a>
                                        <a onclick=\"excluiPeriodo('||aet.aetdid||');\" style=\"cursor: pointer;\" >
                                            <img  src=\"../imagens/excluir.gif\" border=0 title=\"Inativar\">
                                        </a>
                                    </center>' ";
                        }else{
                            $sql.=" CASE WHEN seg.usucpf='".$_SESSION['usucpf']."'
                                     THEN
                                        '<center>
                                            <a href=\"academico.php?modulo=principal/assistenciaEstudantil&acao=A&aetano='||aetano||'&aetsemestre='||aetsemestre||'&evento=A\">
                                                <img src=\"../imagens/alterar.gif\" border=0 title=\"Visualizar\">
                                            </a>
                                            <a onclick=\"excluiPeriodo('||aet.aetdid||');\" style=\"cursor: pointer;\" >
                                                <img  src=\"../imagens/excluir.gif\" border=0 title=\"Inativar\">
                                            </a>
                                        </center>'
                                    ELSE
                                        '<center>
                                            <a href=\"academico.php?modulo=principal/assistenciaEstudantil&acao=A&aetano='||aetano||'&aetsemestre='||aetsemestre||'&evento=A\">
                                                <img src=\"../imagens/alterar.gif\" border=0 title=\"Visualizar\">
                                            </a>
                                        </center>'
                                    END
                                    ";
                        }

                     $sql.= "
                     as a,  aetano,
                    CASE WHEN aetsemestre=1 THEN '1º Semestre'
                    WHEN aetsemestre=2 THEN '2º Semestre'
                    ELSE 'Anual' END as aetsemestre, to_char(aetdtinclusao,'DD/MM/YYYY') as aetdtinclusao , usunome from academico.assistenciaestudantil aet
                    inner join academico.itemassistencia as it on it.aetdid = aet.aetdid
                    inner join seguranca.usuario seg on seg.usucpf = aet.usucpf
                    where aet.aetstatus = 'A' AND aet.entid = {$entid}  ";

                    $cabecalho = array("&nbsp;Ações&nbsp;&nbsp;&nbsp;&nbsp;", "Ano", "Período", "Data","Usuário Responsável");

                    $db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
                    ?></td>
                    </tr>
                    </table>

                </td>
            </tr>
            <tr>
                <td colspan="2" class="subtituloEsquerda">
                    <table width="100%">
                        <tr>
                            <td>
                            Exercício&nbsp;
                            <?php
                            $arDados = array(
                                array('codigo'=>'2010', 'descricao'=>'2010'),
                                array('codigo'=>'2011', 'descricao'=>'2011'),
                                array('codigo'=>'2012', 'descricao'=>'2012'),
                                array('codigo'=>'2013', 'descricao'=>'2013'),
                                array('codigo'=>'2014', 'descricao'=>'2014')
                            );
                            $aetano = $_REQUEST['aetano'];
                            $db->monta_combo('aetano', $arDados, 'S', 'Selecione...', '', '');
                            ?>

                            &nbsp;Semestre/Ano:&nbsp;
                            <?php
                            $arDados = array(
                                array('codigo' => '1', 'descricao' => '1º semestre'),
                                array('codigo' => '2', 'descricao' => '2º semestre')
                            );
                            $aetsemestre = $_REQUEST['aetsemestre'];
                            $db->monta_combo('aetsemestre', $arDados, 'S', 'Selecione...', '', '');

                            //dbg($qrpid);
                            if ($ano && $semestre && $qrpid == -1)
                            {
                            $qrpid = pegaQrpidIndicadores($entid, QUEID_ACADEMICO_INDICADORES, $ano, $semestre);
                            ?>
                                <input type="button" value="Novo" onclick="salvar();">
                            <?php
                            }
                            ?>
                            </td>
                        </tr>
                    </table>
                </td>
            </tr>
        </table>
    </div>
</div>