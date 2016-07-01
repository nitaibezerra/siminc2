<?php
if (!$_SESSION['minc']['mceid'] || !$_SESSION['minc']['entid']) {
    ?>
    <script>
        alert("Sua sessão expirou. Por favor, entre novamente!");
        location.href = 'minc.php?modulo=inicio&acao=A';
    </script>
    <?php
    exit;
}

$entity = array(
    'avaid' => 'NULL',
    'mceid' => 'NULL',
    'avainiciativacultural' => '',
    'avacomentarioiniciativa' => '',
    'avaplanoatividade' => '',
    'avacomentarioplano' => '',
    'avaculturaafro' => '',
    'avaculturaindigena' => '',
    'avamusicanaescola' => ''
);

include_once APPRAIZ . 'includes/workflow.php';

$sqlDocumento = "SELECT * FROM minc.mcemaiscultura WHERE mceid = {$_SESSION['minc']['mceid']}";
$rsDocumento = !empty($_SESSION['minc']['mceid']) ? $db->pegaLinha($sqlDocumento) : array();

if (empty($rsDocumento['docid'])) {
    $docid = wf_cadastrarDocumento(TPDID_MAIS_CULTURA, "Documento Mais Cultura #" . $_SESSION['minc']['mceid']);
    $db->executar("UPDATE minc.mcemaiscultura SET docid = '{$docid}' WHERE mceid = {$_SESSION['minc']['mceid']}");
    $db->commit();
} else {
    $docid = $rsDocumento['docid'];
}
$sqlestado = "SELECT esdid FROM workflow.documento WHERE docid = {$docid}";
$estado = $db->pegaUm($sqlestado);

$perfis = arrayPerfil();

if(($estado == '547')&&!in_array(PERFIL_MINC_ADMINISTRADOR, $perfis)&&!in_array(PERFIL_MINC_MEC, $perfis)&&!$db->testa_superuser()) {
	echo 
		"<script>
			alert('Planos de Atividades Culturais em situação \"Avaliação MEC/MinC\" aguardam classificação diante a possibilidade de recursos PDDE/FNDE suplementares para o Mais Cultura nas Escolas.');
			window.location.href = 'minc.php?modulo=principal/termo_compromisso&acao=A';
		</script>";
	die;
}

if ($_POST) {
    
    if ($estado == '547') {
        $dados = array("esdidorigem" => '547',
            "esdiddestino" => '1104');
        $sqlaedid = "
            SELECT
                aedid
            FROM workflow.acaoestadodoc
            WHERE
                esdidorigem='" . $dados['esdidorigem'] . "'
                AND esdiddestino='" . $dados['esdiddestino'] . "'";

        $aedid = $db->pegaUm($sqlaedid);
        $cmddsc = 'Avaliar Escola';
        $dados_wf = array();
        wf_alterarEstado($docid, $aedid, $cmddsc, $dados_wf);
    }
    $isValid = true;

//      Validando
//      foreach($entity as $key => $value){
//          if($key != 'avaid'){
//              if(!isset($_POST[$key]) || (empty($_POST[$key]) && $_POST[$key] != '0') ){
//                  extract($_POST);
//                  echo "<script>alert('É necessário preencher todos os campos de avaliação!');</script>";;
//                  $isValid = false;
//                  break;
//              }
//          }
//      }

    if ($isValid) {
        if ($_POST['avaid']) {
            $sqlBusca = "SELECT * FROM minc.avaliacaoplanoescola WHERE avaid = {$_POST['avaid']}";
            $result = $db->pegaLinha($sqlBusca);
        } elseif ($_POST['mceid']) {
            $sqlBusca = "SELECT * FROM minc.avaliacaoplanoescola WHERE mceid = {$_POST['mceid']}";
            $result = $db->pegaLinha($sqlBusca);
        }

        if (empty($_POST['mceid']) && empty($_POST['avaid'])) {
            if (empty($_SESSION['minc']['mceid'])) {
                $url = "'minc.php?modulo=principal/lista&acao=A'";
            } else {
                $url = "window.location";
            }
            echo "
                <script>
                    alert('Ocorreu um problema ao salvar a avaliação, tente novamente!');
                    window.location.href = {$url};
                </script>
            ";
            die;
        }

        if ($result['avaid'] != '') {
            $avaid = $result['avaid'];

            #Popula a entidade com os dados do formulario
            foreach ($_POST as $key => $value) {
                if (isset($entity[$key])) {
                    $entity[$key] = $value;
                }
            }

            $avaculturaafro = $entity['avaculturaafro'] != '' ? $entity['avaculturaafro'] : 'f';
            $avaculturaindigena = $entity['avaculturaindigena'] != '' ? $entity['avaculturaindigena'] : 'f';
            $avamusicanaescola = $entity['avamusicanaescola'] != '' ? $entity['avamusicanaescola'] : 'f';

            $sql = "
                UPDATE minc.avaliacaoplanoescola
                    SET avainiciativacultural   = '{$entity['avainiciativacultural']}',
                        avacomentarioiniciativa = '{$entity['avacomentarioiniciativa']}',
                        avaplanoatividade       = '{$entity['avaplanoatividade']}',
                        avacomentarioplano      = '{$entity['avacomentarioplano']}',

                        avaculturaafro          = '{$avaculturaafro}',
                        avaculturaindigena      = '{$avaculturaindigena}',
                        avamusicanaescola       = '{$avamusicanaescola}'

                    WHERE avaid = {$avaid}
                    RETURNING avaid;
            ";
        } else {

            foreach ($_POST as $key => $value) {
                if (isset($entity[$key])) {
                    $entity[$key] = $value;
                }
            }

            $avaculturaafro = $entity['avaculturaafro'] != '' ? $entity['avaculturaafro'] : 'f';
            $avaculturaindigena = $entity['avaculturaindigena'] != '' ? $entity['avaculturaindigena'] : 'f';
            $avamusicanaescola = $entity['avamusicanaescola'] != '' ? $entity['avamusicanaescola'] : 'f';

            $sql = "
                INSERT INTO minc.avaliacaoplanoescola(
                        mceid,
                        avainiciativacultural,
                        avacomentarioiniciativa,
                        avaplanoatividade,
                        avacomentarioplano,
                        avaculturaafro,
                        avaculturaindigena,
                        avamusicanaescola
                    ) VALUES (
                        {$entity['mceid']},
                        '{$entity['avainiciativacultural']}',
                        '{$entity['avacomentarioiniciativa']}',
                        '{$entity['avaplanoatividade']}',
                        '{$entity['avacomentarioplano']}',
                        '{$avaculturaafro}',
                        '{$avaculturaindigena}',
                        '{$avamusicanaescola}'
                    )RETURNING avaid;
            ";
        }

        //ver($sql); die;
        $idAvaliacao = $db->pegaLinha($sql);

        $db->commit();
        extract($entity);
        if ($idAvaliacao) {
            echo "<script>alert('Salvo com sucesso')</script>";
        }
    }
}


// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";

$arMnuid = array();
if ($docid) {
    $sqlDocumento = "select * from workflow.documento where docid = {$docid}";
    $resultDocumento = $db->pegaLinha($sqlDocumento);

    if ($resultDocumento && !in_array($resultDocumento['esdid'], array(/*ESTADO_DOCUMENTO_AVALIACAO, ESTADO_DOCUMENTO_FINALIZADO, ESTADO_DOCUMENTO_AVALIADO, */ESTADO_DOCUMENTO_ENVIADO_FNDE/*, ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR, ESTADO_DOCUMENTO_AVALIACAO*/))) {
        $arMnuid = array(MNUID_AVALIACAO, MNUID_MONITORAMENTO,MNUID_MONITORAMENTO2);
    }
}
$db->cria_aba($abacod_tela, $url, '', $arMnuid);
	

$titulo = "Mais Cultura nas Escolas";
$dsctitulo = 'Avaliação';

monta_titulo($titulo, $dsctitulo);

echo cabecalho();

//carrega dados para edição
$sql = "SELECT ptrid, ptrano, ptrobjetivogeral, ptrjustificativa,
		       ptrresultadosesperados, ptrquantidadealuno,
		       ptrquantidadeprofessor, ptrquantidadepessoa, ptrquantidadefamiliar,
		       ptrproduto, ptrdescricaproduto,descdesenvolvido, descoqueseradesenv, desccomunidadeescolar,
		       desccomunidadelocal, descrelprojetoescola, descelemtodialogo, m.docid, ptrcomunidadelocal
	 	FROM minc.mceplanotrabalho p
	 	INNER JOIN minc.mcemaiscultura m ON m.mceid = p.mceid
	 	WHERE p.mceid = " . $_SESSION['minc']['mceid'];
$dados = $db->pegaLinha($sql);
if ($dados)
    extract($dados);

if ($ptrid) {

    $sql = "SELECT promaterialconsumo, procontservcultural, procontservdiverso, prolocacao, promaterialpermanente
		 	FROM minc.mceprevisaoorcamentaria
		 	WHERE ptrid = $ptrid";
    $dados = $db->pegaLinha($sql);
    if ($dados) {
        extract($dados);
        if ($promaterialconsumo)
            $promaterialconsumo = number_format($promaterialconsumo, 2, ",", ".");
        if ($procontservcultural)
            $procontservcultural = number_format($procontservcultural, 2, ",", ".");
        if ($procontservdiverso)
            $procontservdiverso = number_format($procontservdiverso, 2, ",", ".");
        if ($prolocacao)
            $prolocacao = number_format($prolocacao, 2, ",", ".");
        if ($promaterialpermanente)
            $promaterialpermanente = number_format($promaterialpermanente, 2, ",", ".");
    }
}



if ($_SESSION['minc']['mceid']) {

    // Validando inputs de acordo com o estado do documento
    $sqlDocumento = "SELECT docid
                                    FROM minc.mcemaiscultura
                                    WHERE mceid = {$_SESSION['minc']['mceid']}";
    $rsDocumento = $db->pegaLinha($sqlDocumento);
    $docid = $rsDocumento['docid'];

    if ($docid) {
        $estado = wf_pegarEstadoAtual($docid);

        if (($estado['esdid'] != ESTADO_DOCUMENTO_CADASTRAMENTO) && ($estado['esdid'] != ESTADO_DOCUMENTO_AVALIADO) && ($estado['esdid'] != ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR) && ($estado['esdid'] != ESTADO_DOCUMENTO_AVALIACAO)) {
//         	ver('aqui');
            echo "<script>
                 
                  window.onload = function() {
                  
                        var inputs = document.getElementsByTagName('input');
                        var selects = document.getElementsByTagName('select');
                        var textareas = document.getElementsByTagName('textarea');

                        if (textareas) {
                            for(var i=0; i<textareas.length; i++){
                                textareas[i].disabled = true;
                            }
                        }
                        
                        /*
                        for(var i=0; i<selects.length; i++){
                            selects[i].disabled = true;
                        }
                        */
                        
                        if (inputs) {
                        for(var i=0; i<inputs.length; i++){
                            if( inputs[i].id != 'avacultura' ){
                                inputs[i].disabled = true;
                            }else{
                                inputs[i].disabled = false;
                            }
                        }
                        }
                        
                        document.getElementById('btSalvar').disabled = 0
                        document.getElementById('mceid').disabled = 0
                        document.getElementById('avacomentarioiniciativa').disabled = 0
                        document.getElementById('avacomentarioplano').disabled = 0
                  }
                  </script>
            ";
        }
    }
}

$sql = "SELECT parid, ent.entid, entnumcpfcnpj
			FROM minc.mceparceiro mce
			LEFT JOIN entidade.entidade ent ON ent.entid = mce.entid
			WHERE parstatus='A'
			and mceid = {$_SESSION['minc']['mceid']}";
$rs = $db->pegaLinha($sql);
$parid = $rs['parid'];

if ($parid) {

    $sql = "SELECT * FROM minc.mceparceiro WHERE parstatus='A' and parid = {$parid}";
    $rs = $db->pegaLinha($sql);
    //ver($rs);die;
    if ($rs)
        extract($rs);

    if ($parhistoricoatuacao) {
        $parhistoricoatuacao = trim($parhistoricoatuacao);
        $parhistoricoatuacao = str_replace(chr(10), '', $parhistoricoatuacao);
        $parhistoricoatuacao = str_replace(chr(13), '', $parhistoricoatuacao);
    }


    $sql = "SELECT max(arqid)
			FROM minc.mceanexo
			WHERE parid = {$parid} AND anestatus = 'A'";
    $arqid = $db->pegaUm($sql);

    if ($arqid) {
        $sql = "SELECT *
				FROM public.arquivo
				WHERE arqid = {$arqid}";
        $arquivo = $db->pegaLinha($sql);
        $arquivo['arqnome'] = str_replace(array("'", '"'), '', $arquivo['arqnome']);
    }
}

$arrAvaliacao = array();
$arrAvaliacao[] = array('codigo' => '0', 'descricao' => '0');
$arrAvaliacao[] = array('codigo' => '1', 'descricao' => '1');
$arrAvaliacao[] = array('codigo' => '2', 'descricao' => '2');

if ($mceid && count($_POST) === 1 && array_key_exists('theme_simec', $_POST)) {
    $sqlBusca = "SELECT * FROM minc.avaliacaoplanoescola WHERE mceid = {$mceid}";
    $result = $db->pegaLinha($sqlBusca);
    if ($result) {
        extract($result);
    }
}


$pode = !in_array(PERFIL_MINC_CADASTRADOR, $perfis)&&!in_array(PERFIL_MINC_SUPER_USUARIO, $perfis);

$habilita = 'N';
$bloq = 'S';
if($pode){
	$habilita = 'S';
	$bloq = 'N';
}
require_once APPRAIZ . 'includes/classes/entidades.class.inc';
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form id="formulario" name="formulario" action="" method="post">
    <table border="0" width="95%" align="center"  cellpadding="2" cellspacing="1">
        <input id="mceid" name="mceid" type="hidden" value="<?php echo $mceid ?>" />
        <tr>
            <td class="SubTituloDireita">Atividades</td>
            <td>
                <input type="checkbox" <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?> name="avaculturaafro" id="avacultura" <? if ($avaculturaafro == 't') echo 'checked = checked'; ?> value="t"> <b> Cultura Afro Brasileira </b>
                <br>
                <input type="checkbox" <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?> name="avaculturaindigena" id="avacultura" <? if ($avaculturaindigena == 't') echo 'checked = checked'; ?> value="t"> <b> Culturas Indígenas  </b>
                <br>
                <input type="checkbox" <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?> name="avamusicanaescola" id="avacultura" <? if ($avamusicanaescola == 't') echo 'checked = checked'; ?> value="t"> <b> Música nas Escolas </b>
            </td>
        </tr>
        <tr>
            <td class="SubTituloCentro" style="background-color: #dcdcdc;" colspan="4"></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Parceiro</td>
            <td>
                <?php
                $entidade = new Entidades();

                if (!empty($_SESSION['avaliacao']['entidade'])) {
                    $entidade->carregarPorEntid($_SESSION['avaliacao']['entidade']);
                }

                switch ($_SESSION['avaliacao']['tipo_entidade']) {
                    case 1:
                        echo $entidade->formEntidade("minc.php?modulo=principal/avaliacao&acao=A", array("funid" => FUN_PARCEIRO_MC_PF, "entidassociado" => $_SESSION['minc']['entid']), array("enderecos" => array(1), "obrigatorios" => array("entnumcpfcnpj", "entnome")), "A",'S');
                        break;
                    case 2:
                        echo $entidade->formEntidade("minc.php?modulo=principal/avaliacao&acao=A", array("funid" => FUN_PARCEIRO_MC_PJ, "entidassociado" => $_SESSION['minc']['entid']), array("enderecos" => array(1), "obrigatorios" => array("entnumcpfcnpj", "entnome")), "A",'S');
                        break;
                    default:
                        echo("Problemas com a função da entidade");
                }
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloCentro" style="background-color: #dcdcdc;" colspan="4">Iniciativa Cultural Parceira</td>
        </tr>
        <tr >
            <td class="SubTituloDireita" width="30%">
                <strong>Histórico de atuação </strong>
                <br />
                <br />Detalhar experiências anteriores da Iniciativa Cultural Parceira, relacionadas ao Plano de Atividade Cultural da Escola proposto e aos eixos temáticos escolhidos. Nesse campo podem ser ofercidos endereços eletrônicos por meio dos quais seja possível tomar conhecimento de registros e/ou trabalhos atuais  da Iniciativa Cultural Parceira.
            </td>
            <td>
                <?php echo $parhistoricoatuacao ?>
            </td>
        </tr>
        <tr id="tr_anexo"><td class="SubTituloDireita">
                <strong>Anexo do Portfólio</strong><br /><br />Portfólios são arquivos que por meio de textos e imagens expõe os trabalhos desenvolvidos pelo artista/ iniciativa cultural parceira de maneira objetiva. Podem ser anexados arquivos em formato PDF de até 3MB.</td>
            <td>
                <a href="minc.php?modulo=principal/parceiros&acao=A&requisicao=downloadArquivo&arqid=<?php echo $arquivo['arqid']; ?>"><?php echo $arquivo['arqnome'] . '.' . $arquivo['arqextensao']; ?></a>
            </td>
        </tr>
        <?if($pode){?>
        <tr >
            <td class="SubTituloDireita">
                <strong>Avaliação: </strong>
            </td>
            <td>
                <?php if (($resultDocumento['esdid'] != ESTADO_DOCUMENTO_CADASTRAMENTO) && ($resultDocumento['esdid'] != ESTADO_DOCUMENTO_AVALIADO) && ($resultDocumento['esdid'] != ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR) && ($resultDocumento['esdid'] != ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR) && ($resultDocumento['esdid'] != ESTADO_DOCUMENTO_AVALIACAO) && ($resultDocumento['esdid'] != ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR) && ($resultDocumento['esdid'] != ESTADO_DOCUMENTO_AVALIACAO)): ?>
                    <?php echo $avainiciativacultural; ?>
                <?php else: ?>
                    <?php echo $db->monta_combo("avainiciativacultural", $arrAvaliacao, 'S', "Avaliar", "", "", "", "", "S", "avainiciativacultural", "", ''); ?>
                <?php endif; ?>
            </td>
        </tr>
        <?}?>
        <tr >
            <td class="SubTituloDireita">
                <strong>Descrição: </strong>
            </td>
            <td>
                <?php if (($resultDocumento['esdid'] != ESTADO_DOCUMENTO_CADASTRAMENTO) && ($resultDocumento['esdid'] != ESTADO_DOCUMENTO_AVALIADO) && ($resultDocumento['esdid'] != ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR) && ($resultDocumento['esdid'] != ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR) && ($resultDocumento['esdid'] != ESTADO_DOCUMENTO_AVALIACAO)): ?>
                    <?php echo $avacomentarioiniciativa; ?>
                <?php else: ?>
                    <?php echo campo_textarea('avacomentarioiniciativa', 'N', $habilita, '', 64, 05, 5000, '', '', '', '', 'id="avacomentarioiniciativa"'); ?>
                <?php endif; ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloCentro" colspan="4">Descrição do Plano de Atividade Cultural</td>
        </tr>
        <tr>
            <td width="20%" class="SubTituloDireita"><b>Eixo Temático:</b></td>
            <td colspan="2" >
                <?php
                $sql = "SELECT extid, extdescricao, extexplicacao FROM minc.mceeixotematico WHERE extstatus = 'A' ORDER BY 2";
                $eixo = $db->carregar($sql);
                foreach ($eixo as $a) {
                    $checado = '';
                    if ($ptrid) {
                        $checado = $db->pegaUm("SELECT extid FROM minc.mceplanoeixo WHERE ptrid = {$ptrid} AND extid = " . $a['extid']);
                        if ($checado)
                            $checado = 'checked';
                    }
                    ?>
                    <p align="justify">&nbsp;
                        <b>
                            <?php if ($checado): ?>
                                <img border="0" title="Indica campo obrigatório." src="../imagens/check_p.gif">
                            <?php else: ?>
                                <!--<img border="0" title="Indica campo obrigatório." src="../imagens/check_p_01.gif">-->
                            <?php endif; ?>
                            <!--<input id="chk_eixo" type="checkbox" name="chk_eixo[]" value="' . $a['extid'] . '" <?php echo $checado ?>>&nbsp;-->
                            <?php echo $a['extdescricao'] ?>:</b>
                        <?php echo $a['extexplicacao'] ?>
                    </p>
                    <?php
                }
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Objetivo Geral:</b><br>
                <p align="justify">(Nesse campo abordar o que o projeto como um todo quer alcançar, mobilizar ou proporcionar à escola e à comunidade em que ela está inserida, para além dos objetivos específicos visados em cada atividade proposta. Por exemplo: oficinas de teatro podem ter por objetivo específico familiarizar participantes com expressão corporal ou textual, a depender de sua abordagem; já o Plano de Atividade Cultural pode ter por objetivo montar uma peça sobre as origens do bairro em que a escola está localizada e, assim sendo, seu objetivo geral não é a realização da peça propriamente dita, mas desenvolver um processo de trabalho e aprendizado que proporcione apropriação do lugar pela escola/ comunidade.)</p>
            </td>
            <td colspan="2">
                <?php echo campo_textarea('txa_objgeral', 'N', 'N', 'Objetivo Geral', 70, 8, '', '', 0, '', false, '', $ptrobjetivogeral) ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Justificativa:</b>
                <p align="justify">(Descreva o contexto e a realidade sociocultural e econômica da comunidade em que a escola está inserida e justifique a importância do desenvolvimento do projeto. Fale sobre as peculiaridades da escola, da iniciativa cultural parceira e do público a que se destina o projeto.)</p>
            </td>
            <td colspan="2" >
                <?php echo campo_textarea('txa_just', 'N', 'N', 'Justificativa', 70, 8, '', '', 0, '', false, null, $ptrjustificativa) ?>
            </td>
        </tr>
        <tr>
            <td colspan="4" class="subtituloCentro">METODOLOGIA</td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>O que será desenvolvido:</b></td>
            <td colspan="2" >
                <?php echo campo_textarea('txa_desenv', 'N', 'N', 'METODOLOGIA', 70, 8, '', '', 0, '', false, null, $descdesenvolvido) ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Como será desenvolvido:</b></td>
            <td colspan="2" >
                <?php echo campo_textarea('txa_desenvolvido', 'N', 'N', 'Como será desenvolvido', 70, 8, '', '', 0, '', false, null, $descoqueseradesenv) ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Como a comunidade escolar (pais, alunos, professores e funcionários da escola) será integrada ao projeto:</b></td>
            <td colspan="2" >
                <?php echo campo_textarea('txa_comunid', 'N', 'N', 'Como a comunidade escolar (pais, alunos, professores e funcionários da escola) será integrada ao projeto', 70, 8, '', '', 0, '', false, null, $desccomunidadeescolar) ?>
            </td>
        </tr>
        <!--
                <tr>
                        <td class="SubTituloDireita"><b>Haverá envolvimento da comunidade local (moradores, estabelecimentos comerciais, espaços públicos, espaços coletivos diversos que se encontram entorno da escola) com o projeto?</b></td>
                        <td colspan="3"><//?= campo_textarea('desccomunidadelocal', 'S', 'S', '', 80, 5, 1500); ?></td>
                </tr>
        -->
        <tr>
            <td class="SubTituloDireita"><b>Haverá envolvimento da comunidade local (moradores, estabelecimentos comerciais, espaços públicos, espaços coletivos diversos que se encontram entorno da escola) com o projeto?</b></td>
            <td colspan="2" >
                <input type="radio" <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?> name="ptrcomunidadelocal" value="S" <?php echo $ptrcomunidadelocal == 't' ? 'checked' : '' ?>>&nbsp;Sim&nbsp;
                <input type="radio" <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?> name="ptrcomunidadelocal" value="N" <?php echo $ptrcomunidadelocal == 'f' ? 'checked' : '' ?>>&nbsp;Não
            </td>
        </tr>
        <tr id="tr_envolvimento">
            <td class="SubTituloDireita"><b>Envolvimento:</b></td>
            <td  colspan="2">
                <?php echo $desccomunidadelocal ?>
            </td>
        <tr>
            <td class="SubTituloDireita"><b>Qual a relação entre o projeto proposto e o projeto político pedagógico da escola:</b></td>
            <td  colspan="2"><?php echo $descrelprojetoescola ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Haverá produto ao final desta parceria entre a escola e a iniciativa parceira? (exposição, vídeo, peça teatral, etc):</b></td>
            <td  colspan="2">
                <input type="radio" <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?> name="ptrproduto" value="S" <?php echo $ptrproduto == 't' ? 'checked' : '' ?>>&nbsp;Sim&nbsp;
                <input type="radio" <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?> name="ptrproduto" value="N" <?php echo $ptrproduto == 'f' ? 'checked' : '' ?>>&nbsp;Não
            </td>
        </tr>
        <tr id="tr_descricao_produto">
            <td class="SubTituloDireita"><b>Descreva o produto:</b></td>
            <td colspan="2">
                <?php echo $ptrdescricaproduto ?>
            </td>
        </tr>
<!-- 		<tr>
                <td class="SubTituloDireita"><b>Elementos do  diálogo entre o plano de trabalho proposto e o projeto pedagógico da escola:</b></td>
                <td colspan="3"><? //= campo_textarea('descelemtodialogo', 'S', 'S', '', 80, 5, 1000);  ?></td>
        </tr> -->
        <tr>
            <td colspan="4" class="subtituloCentro">PLANO DE TRABALHO DETALHADO</td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Estimativa de Pessoas Envolvidas:</b></td>
            <td  colspan="2" >
                <table class="listagem" style="width: 100%">
                    <tr class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color: #dcdcdc;">
                        <td><b>Envolvidos</b></td>
                        <td><b>Quantidade</b></td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Estudantes</td>
                        <td><?php echo $ptrquantidadealuno ?></td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Professores</td>
                        <td><?php echo $ptrquantidadeprofessor ?></td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Familiares</td>
                        <td><?php echo $ptrquantidadefamiliar ?></td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Pessoas da Comunidade</td>
                        <td><?php echo $ptrquantidadepessoa ?></td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>
                            Total
                        </td>
                        <td>
                            <?
                            if ($ptrquantidadealuno)
                            $totalenvolvidos += $ptrquantidadealuno;
                            if ($ptrquantidadeprofessor)
                            $totalenvolvidos += $ptrquantidadeprofessor;
                            if ($ptrquantidadefamiliar)
                            $totalenvolvidos += $ptrquantidadefamiliar;
                            if ($ptrquantidadepessoa)
                            $totalenvolvidos += $ptrquantidadepessoa;
                            echo $totalenvolvidos;
                            ?>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Resultados Esperados:</b></td>
            <td colspan="2" ><?php echo $ptrresultadosesperados ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Cronograma de Ação:</b>
                <p align="justify">(O cronograma complementa o que foi exposto no item metodologia de trabalho e deve conter a atividade (o que) e o período (quando), pelo período mínimo de 6 (seis) meses letivos, ainda que não consecutivos, a contar do mês da efetivação do repasse de recursos.)</p>
            </td>
            <td colspan="2" >
                <table  style="width: 100%">
                    <tr>
                        <td>
                            <table id="tblcronograma" class="listagem"  style="width: 100%">
                                <tr class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color: #dcdcdc;">
                                    <td colspan="13" align="center">
                                        <b>CRONOGRAMA <?= $_SESSION['exercicio'] + 1 ?></b>
                                    </td>
                                </tr>
                                <tr class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color: #dcdcdc;">
                                    <td >
                                        <b>O que?</b>
                                    </td>
                                    <? for ($i = 1; $i < 13; $i++) { ?>
                                    <td>
                                        &nbsp;
                                        <b><?= substr(mes_extenso($i), 0, 3) ?></b>
                                        &nbsp;
                                    </td>
                                    <? } ?>
                                </tr>
                                <?
                                if ($ptrid) {
                                //$sql = "SELECT craid, ptrid, cradescricao, crames, craano FROM minc.mcecronogramaatividade where ptrid = $ptrid";
                                $sql = "SELECT cradescricao
                                FROM minc.mcecronogramaatividade
                                where ptrid = $ptrid
                                and craano = " . $_SESSION['exercicio'] . "
                                group by cradescricao
                                order by 1";
                                $cronograma = $db->carregar($sql);
                                }

                                $ctCronograma = count($cronograma);
                                if ($ctCronograma <= 1)
                                $ctCronograma = 1;

                                for ($ct = 1; $ct <= $ctCronograma; $ct++) {
                                ?>
                                <tr>
                                    <td>
                                        <?= campo_texto('cradescricao[' . $ct . ']', 'N', 'S', $cronograma[$ct - 1]['cradescricao'], 50, 200, '', '', '', '', '', '', '', $cronograma[$ct - 1]['cradescricao']); ?>
                                    </td>
                                    <?
                                    for ($i = 1; $i < 13; $i++) {

                                    if ($ptrid) {
                                    $sql = "SELECT crames
                                    FROM minc.mcecronogramaatividade
                                    where ptrid = $ptrid
                                    and craano = " . $_SESSION['exercicio'] . "
                                    and cradescricao = '" . addslashes($cronograma[$ct - 1]['cradescricao']) . "'
                                    and crames = $i";
                                    $mes = $db->pegaUm($sql);
                                    }

                                    $checado = '';
                                    if ($mes)
                                    $checado = 'checked';
                                    ?>
                                    <td>
                                        <input type="checkbox" <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?> name="crames_<?= $ct ?>[]" id="crames_<?= $ct ?>_<?= $i ?>" value="<?= $i ?>" <?= $checado ?>>
                                    </td>
                                    <? } ?>
                                </tr>
                                <? } ?>
                            </table>
                        </td>
                        <td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Previsão Orçamentária:</b></td>
            <td colspan="2">
                <?php
                $sql = "select e.fk_cod_entidade as codibge,count(m.fk_cod_aluno) as qtd_alunos
                        from educacenso_2013.tab_dado_escola e
                        inner join educacenso_2013.tab_matricula m ON e.fk_cod_entidade = m.fk_cod_entidade
                        join entidade.entidade ee on ee.entcodent::char varying = e.fk_cod_entidade::char varying
                        where ee.entid IN ({$_SESSION['minc']['entid']})
                        group by 1";

                //ver($_SESSION['minc']);
                $mcequantidadealuno = $db->pegaLinha($sql);

                if ($mcequantidadealuno['qtd_alunos'] <= 500) {
                    $saldoorcamento = 20000;
                    $promaterialpermanente = 2000;
                } else
                if ($mcequantidadealuno['qtd_alunos'] >= 501 && $mcequantidadealuno['qtd_alunos'] <= 1000) {
                    $saldoorcamento = 21000;
                    $promaterialpermanente = 2500;
                } else
                if ($mcequantidadealuno['qtd_alunos'] > 1000) {
                    $saldoorcamento = 22000;
                    $promaterialpermanente = 3000;
                } else {
                    $saldoorcamento = 0;
                    $promaterialpermanente = 0;
                }
                ?>
                <table  style="width: 100%" class="listagem" border="0">
                    <tr class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color: #dcdcdc;">
                        <td><b>O que</b></td>
                        <td width="37%"><b>Valor</b></td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Total Orçamento</td>
                        <td>
                            <?php
                            $totalorcamento = number_format($saldoorcamento, 2, ",", ".");
                            echo $totalorcamento;
                            ?>
                        </td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Aquisição de Materiais de consumo</td>
                        <td><?php echo $promaterialconsumo; ?></td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Contratação de serviços culturais necessários às atividades artísticas e pedagógicas</td>
                        <td><?php echo $procontservcultural; ?></td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Contratação de serviços diversos</td>
                        <td><?php echo $procontservdiverso; ?></td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Locação de instrumentos, transporte, equipamentos</td>
                        <td><?php echo $prolocacao; ?></td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Aquisição de materiais permanentes e equipamentos</td>
                        <td>
                            <?php
                            $promaterialpermanente = number_format($promaterialpermanente, 2, ",", ".");
                            echo $promaterialpermanente;
                            ?>
                        </td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Saldo Orçamento</td>
                        <td>
                            <?php
                            if ($promaterialconsumo)
                                $saldoorcamento -= str_replace(",", ".", str_replace(".", "", $promaterialconsumo));
                            if ($procontservcultural)
                                $saldoorcamento -= str_replace(",", ".", str_replace(".", "", $procontservcultural));
                            if ($procontservdiverso)
                                $saldoorcamento -= str_replace(",", ".", str_replace(".", "", $procontservdiverso));
                            if ($prolocacao)
                                $saldoorcamento -= str_replace(",", ".", str_replace(".", "", $prolocacao));
                            if ($promaterialpermanente)
                                $saldoorcamento -= str_replace(",", ".", str_replace(".", "", $promaterialpermanente));
                            $saldoorcamento = number_format($saldoorcamento, 2, ",", ".");
                            echo $saldoorcamento;
                            ?>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
        <?if($pode){?>
        <tr >
            <td class="SubTituloDireita">
                <strong>Avaliação: </strong>
            </td>
            <td>
                <?php if (($resultDocumento['esdid'] != ESTADO_DOCUMENTO_CADASTRAMENTO) && ($resultDocumento['esdid'] != ESTADO_DOCUMENTO_AVALIADO) && ($resultDocumento['esdid'] != ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR) && ($resultDocumento['esdid'] != ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR) && ($resultDocumento['esdid'] != ESTADO_DOCUMENTO_AVALIACAO)): ?>
                    <?php echo $avaplanoatividade; ?>
                <?php else: ?>
                    <?php echo $db->monta_combo("avaplanoatividade", $arrAvaliacao, 'S', "Avaliar", "", "", "", "", "S", "avaplanoatividade", "", ''); ?>
                <?php endif; ?>
            </td>
        </tr>
        <?}?>
        <tr >
            <td class="SubTituloDireita">
                <strong>Descrição: </strong>
            </td>
            <td>
                <?php if (($resultDocumento['esdid'] != ESTADO_DOCUMENTO_CADASTRAMENTO) && ($resultDocumento['esdid'] != ESTADO_DOCUMENTO_AVALIADO) && ($resultDocumento['esdid'] != ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR) && ($resultDocumento['esdid'] != ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR) && ($resultDocumento['esdid'] != ESTADO_DOCUMENTO_AVALIACAO)): ?>
                    <?php echo $avacomentarioplano; ?>
                <?php else: ?>
                    <?php echo campo_textarea('avacomentarioplano', 'N',$habilita, '', 64, 05, 5000, '', '', '', '', 'id="avacomentarioplano"'); ?>
                <?php endif; ?>
            </td>
        </tr>
        <?if(!$pode){?>
        <tr>
        	<td class="SubTituloDireita">
        		<strong>Conceito:</strong>
        	</td>
        	<td>
        		<?
        			if( ($avaplanoatividade == '1' && $avainiciativacultural == '2') ||($avaplanoatividade == '2'  && $avainiciativacultural == '1')||($avaplanoatividade == '2'  && $avainiciativacultural == '2')){
				?>
        				<strong>A</strong>
        		<?
        			}elseif( ($avaplanoatividade == '1' && $avainiciativacultural == '1')||($avaplanoatividade == '2'  && $avainiciativacultural == '0')or ($avaplanoatividade == '0'  && $avainiciativacultural == '2')){
        		?>
        				<strong>B</strong>
        		<?		
        			}elseif(($avaplanoatividade == '0' && $avainiciativacultural == '0')||($avaplanoatividade == '1'  && $avainiciativacultural == '0')or ($avaplanoatividade == '0'  && $avainiciativacultural == '1')){
        		?>
        				<strong>C</strong>
        		<?	
        			}
        		?>
        	</td>
        </tr>
        <tr >
            <td class="SubTituloDireita">
                <strong>O que são os conceitos A/B/ C? </strong>
            </td>
            <td>
					<strong>Planos de Atividade Culturais avaliados com o conceito A</strong> são aqueles que atendem e/ ou superam objetivos do Mais Cultura nas Escolas, considerados no processo de seleção, a saber: 
					diálogo entre experiência do(a) parceiro(a) cultural e Plano de Atividade apresentado; aprofundamento em relação às propostas já desenvolvidas por meio dos programas Mais Educação e 
					Ensino Médio Inovador (SEB-MEC); valorização de práticas artísticas e culturais não hegemônicas ou não predominantes, embora representativas da realidade do território em que a escola 
					está inserida; valorização de práticas artísticas e culturais não hegemônicas ou não predominantes na cultura de massas (por exemplo, repertório musical não presente e/ ou que não recebe 
					destaque nos meios de comunicação em massa); aprofundamento do repertório cultural da comunidade escolar, em especial abrangendo outros atores além de estudantes, docentes, por exemplo.
					<br>
					<br>
					<strong>Planos de Atividade Culturais avaliados com o conceito B</strong> são aqueles que atendem critérios considerados indispensáveis no processo de seleção, a saber: algum nível de aprofundamento 
					em relação às propostas já desenvolvidas por meio dos programas Mais Educação e Ensino Médio Inovador (SEB-MEC); indicação de repertórios e/ ou práticas artísticas e culturais que 
					apresentem potencial significativo, seja em relação à ampliação do repertório cultural da comunidade escolar, seja em relação à experimentação nas práticas pedagógicas da escola; 
					Planos de Atividades Culturais priorizados em função de acordo com parceria MinC/MDS. 
					<br>
					<br>
					<strong>Planos de Atividades Culturais avaliados com C</strong> são aqueles que não atendem critérios considerados indispensáveis no processo de seleção, a saber: não apresentam aprofundamento em 
					relação às propostas já desenvolvidas por meio dos programas Mais Educação e Ensino Médio Inovador (SEB-MEC); apresentam parceiros(as) culturais cuja experiência é incompatível com
					Plano de Atividade apresentado.
            </td>
        </tr>
        <tr>
        	<td class="SubTituloCentro" colspan = '2'>
                Orientações Gerais para Execução dos Recursos e Desenvolvimento dos Planos de Atividades Culturais 
            </td>
        </tr>
        <tr>
        	<td colspan = '2'>
            		<br>
					<br>
					O repasse dos recursos do Mais Cultura nas Escolas para as escolas cujos Planos de Atividades Culturais foram selecionados, começou em maio de 2014, e estão sendo efetuados por meio de 
					depósito na conta do PDDE/ Qualidade - Ação Mais Cultura nas Escolas. Conforme define a Resolução FNDE/ PDDE n° 10 de 18/04/2013, o FNDE realizará a abertura da mesma. Os repasses podem 
					ser acompanhados por meio do endereço <a>www.fnde.gov.br</a>, no ícone “Sistema de Consulta à Liberação de Recursos dos Programas do FNDE”.  Conforme definido pela Resolução FNDE/PDDE n° 5 de 
					31/03/2014, os repasses do ano exercício de 2014 serão efetuados em 2 (duas) parcelas.
					<br>
					<br>
					<br>
					<br>
					As observações da Avaliação MEC/MinC deverão ser consideradas no replanejamento do Plano de Atividades, em alguns casos também durante o desenvolvimento do mesmo. Salienta-se que agentes 
					públicos na ativa e entidades públicas não podem receber recursos do PDDE/ FNDE, sob risco de impedimento da vinculação da escola e/ ou da administração pública local, ao PDDE/ FNDE. 
					Os cronogramas de execução deverão ser revistos. Recomenda-se replanejamento e inicio imediato das atividades, uma vez disponibilizada a primeira parcela recursos para desenvolvimento dos 
					Planos de Atividades. Durante a execução do Plano de Atividade Cultural é indispensável que as escolas atualizem o uso dos recursos repassados por meio da aba “Monitoramento”, que será 
					disponibilizada em breve no módulo “Mais Cultura” do SIMEC. Caso necessário, as atividades poderão ser estendidas ao próximo ano letivo, contudo, deve-se atentar às normas PDDE/ FNDE para 
					uso dos recursos no ano exercício em que foram repassados, sob pena de redução dos mesmos no ano seguinte. <strong>Desse modo, recomenda-se que o replanejamento dos das atividades considere meios de 
					fazer uso dos recursos, acordando e, se possível, adianto compras e possíveis pagamentos de serviços tão logo os recursos seja disponibilizados, sem prejuízo de atendimento as regras PDDE/ FNDE 
					para uso dos mesmos (Resolução PDDE/ FNDE n° 10 de 18/04/2013).</strong>
					<br>
					<br>
					<br>
					<br>
					Para outras informações e dúvidas escrever para <a>maisculturanasescolas@cultura.gov.br</a>. Acompanhe também informações atualizadas periodicamente no endereço virtual <a>www.cultura.gov.br/maisculturanasescolas</a>.
            </td>
        </tr>
        <?}?>
        <tr>
            <td align="center" width="100%" style="background: rgb(238, 238, 238);" colspan="4">
                <table width="100%">
                    <tr>
                        <td class="SubTituloCentro" colspan="2">
                            <?php
                            if ($estado['esdid'] != ESTADO_DOCUMENTO_ENVIADO_FNDE && $pode) {
                                echo "<input type=\"button\" name=\"btSalvar\" id=\"btSalvar\" value=\"Salvar\">";
                            }
//									else{
//                                     echo "<input type=\"button\" name=\"btSalvarC1\" id=\"btSalvarC1\" value=\"Salvar\" disabled=\"disabled\">";
//                                 }

                            ?>
                        </td>
                        <td align="right">&nbsp;</td>
                    </tr>
                </table>
            </td>
        </tr>
    </table>
</form>
<script lang="javascript">
jQuery(document).ready(function(){
    
	jQuery('[name*=ava]').attr('disabled',true);
    
    <?php if(in_array(PERFIL_MINC_CADASTRADOR, $perfis) || in_array(PERFIL_MINC_SUPER_USUARIO, $perfis)){ ?>
    	jQuery('[name*=ava]').attr('disabled',false);
    <?php } ?>
        
});
    jQuery('#btSalvar').click(function() {
        var avainiciativacultural = jQuery('#avainiciativacultural');
        var avacomentarioiniciativa = jQuery('#avacomentarioiniciativa');
        var avaplanoatividade = jQuery('#avaplanoatividade');
        var avacomentarioplano = jQuery('#avacomentarioplano');

        if (avainiciativacultural.val() == '') {
            alert('O campo "Avaliação" do "Iniciativa Cultural Parceria"  é necessário!');
            avainiciativacultural.focus();
            return false;
        }

//         if(avacomentarioiniciativa.val() == ''){
//             alert('O campo "Descrição" do "Iniciativa Cultural Parceria"  é necessário!');
//             avacomentarioiniciativa.focus();
//             return false;
//         }

        if (avaplanoatividade.val() == '') {
            alert('O campo "Avaliação" do "Plano de Atividade Cultural da Escola 2013"  é necessário!');
            avaplanoatividade.focus();
            return false;
        }

//         if(avacomentarioplano.val() == ''){
//             alert('O campo "Descrição" do "Plano de Atividade Cultural da Escola 2013"  é necessário!');
//             avacomentarioplano.focus();
//             return false;
//         }

        jQuery('#formulario').submit();
    })

</script>
