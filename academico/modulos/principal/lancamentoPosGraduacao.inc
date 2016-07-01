<?php

// cabecalho da app
require_once APPRAIZ . "includes/cabecalho.inc";
require_once APPRAIZ . "academico/classes/NivelCurso.class.inc";
require_once APPRAIZ . "academico/classes/CursoPosGraduacao.class.inc";
require_once APPRAIZ . "academico/classes/Liberacao.class.inc";
require_once APPRAIZ . "academico/classes/LiberacaoIndividual.class.inc";
require_once APPRAIZ . "academico/classes/Lancamento.class.inc";
echo '<br/>';

$abacod_tela = ($_SESSION['baselogin'] == "simec_desenvolvimento") ? ABA_CURSO_POSGRADUACAO_DEV : ABA_CURSO_POSGRADUACAO_OLD; //Dados Desenv

$db->cria_aba( $abacod_tela , $url, array() );

monta_titulo("Programa de Pós-Graduação", "");

$oNivelCurso 			= new NivelCurso();
$oCursoPosGraduacao 	= new CursoPosGraduacao();
$oLancamento 			= new Lancamento();
$oLiberacao 			= new Liberacao();
$oLiberacaoIndividual	= new LiberacaoIndividual();

// recupera os niveis de curso cadastrados
$niveisCurso = $oNivelCurso->lista();

$entid = $_GET['entid'] ? $_GET['entid'] : $_SESSION['academico']['entid'];

// cria o cabeçalho padrão do módulo
$autoriazacaoconcursos = new autoriazacaoconcursos();
echo $autoriazacaoconcursos->cabecalho_entidade($entid);

if(isset($_POST['lct'])){

    $lctid = $oLancamento->salvar($_POST['lct'][$_POST['nivel']], $_POST['nivel'], $entid);

    if($lctid){

        echo "<script type='text/javascript'> 
        alert('Salvo com Sucesso!'); 
        location.href = '?modulo=principal/lancamentoPosGraduacao&acao=A&entid={$_GET['entid']}'; 
        </script>";

    }else{

        alert('O lançamento não pode ultrapassar o Limite de Vagas');
        extract($_POST);

    }
    
}else if(isset($_GET['excluir'])){

    $oLancamento->excluir($_GET['cpgid']);

}else if(isset($_GET['editar'])){

    $arrDados = $oLancamento->editar($_GET['cpgid']);
    $cpgid = $_GET['cpgid'];
    extract($arrDados);

}


?>

<script type="text/javascript" src="/includes/prototype.js"></script>

<script type="text/javascript">

    function incluir(nvcid){

        if($('lct' + nvcid + 'cpgid').getValue().trim() == ''){

            alert('É necessário selecionar o curso para efetuar o Lançamento');

        }else{

            $('nivel').setValue(nvcid);
            $('form').submit();

        }

    }

    function editar(cpgid){

        location.href = "?modulo=principal/lancamentoPosGraduacao&acao=A&entid=<?php echo $_GET['entid']; ?>&editar=ok&cpgid=" + cpgid;

    }

    function excluir(cpgid){

        if(confirm('Deseja realmente excluir este lançamento?'))
            location.href = "?modulo=principal/lancamentoPosGraduacao&acao=A&entid=<?php echo $_GET['entid']; ?>&excluir=ok&cpgid=" + cpgid;

    }

</script>

<style type="text/css">

    .verde td{
        font-weight: bold;
        text-align: center;
        color: green;
    }

    .vermelho td{
        font-weight: bold;
        text-align: center;
        color: red;
    }

</style>

<form action="" method="post" name="form" id="form">

    <!-- campo utilizado para salvar apenas o lancamento acionado -->
    <input type="hidden" name="nivel" id="nivel" value="" />

    <?php foreach($niveisCurso as $nivel): ?>

        <div id="abas" style="display: block;">
            <div class="abaMenu">
                <ul id="listaAbas">
                    <li class="abaItemMenu" style="width:200px;">
                            <?php echo $nivel['nvcdsc']; ?>
                    </li>
                </ul>
            </div>

            <div id="aba_docente" class="conteudoAba" style="padding-top:2px;padding-bottom: 0px;">

            <table width="100%" align="center" border="0" cellspacing=   "0" cellpadding="2" class="listagem">
                <thead>
                    <tr>
                        <th> Curso </th>
                        <th> 2008 </th>
                        <th> 2009 </th>
                        <th> 2010 </th>
                        <th> 2011 </th>
                        <th> 2012 </th>
                        <th> A&ccedil;&atilde;o </th>
                    </tr>
                </thead>
                <tbody>

                    <?php if( ($oLiberacao->noPrazo() || $oLiberacaoIndividual->noPrazo( $entid )) && ( $db->testa_superuser() || in_array(PERFIL_IFES_CADASTRO_PROGRAMA, pegaArrayPerfil($_SESSION['usucpf']) ) ) ): ?>
                    <tr>
                        <td> <?php $oCursoPosGraduacao->montaComboPorNivel($entid, $nivel['nvcid'], "lct[{$nivel['nvcid']}][cpgid]", "lct{$nivel['nvcid']}cpgid", $cpgid); ?> </td>
                        <td align="center"> <?php echo campo_texto("lct[{$nivel['nvcid']}][2008]", 'N', 'S', '', 4, 4, '####', '', '', '', '', "id='lct{$nivel['nvcid']}2008'", '', $lct[$nivel['nvcid']]['2008']); ?> </td>
                        <td align="center"> <?php echo campo_texto("lct[{$nivel['nvcid']}][2009]", 'N', 'S', '', 4, 4, '####', '', '', '', '', "id='lct{$nivel['nvcid']}2008'", '', $lct[$nivel['nvcid']]['2009']); ?> </td>
                        <td align="center"> <?php echo campo_texto("lct[{$nivel['nvcid']}][2010]", 'N', 'S', '', 4, 4, '####', '', '', '', '', "id='lct{$nivel['nvcid']}2008'", '', $lct[$nivel['nvcid']]['2010']); ?> </td>
                        <td align="center"> <?php echo campo_texto("lct[{$nivel['nvcid']}][2011]", 'N', 'S', '', 4, 4, '####', '', '', '', '', "id='lct{$nivel['nvcid']}2008'", '', $lct[$nivel['nvcid']]['2011']); ?> </td>
                        <td align="center"> <?php echo campo_texto("lct[{$nivel['nvcid']}][2012]", 'N', 'S', '', 4, 4, '####', '', '', '', '', "id='lct{$nivel['nvcid']}2008'", '', $lct[$nivel['nvcid']]['2012']); ?> </td>
                        <td align="center">
                            <a href="javascript: incluir(<?php echo $nivel['nvcid']; ?>);" title="Incluir" style="border: none;">
                                <img src="../imagens/gif_inclui.gif" border="0" align="absmiddle" alt="Incluir" />
                            </a>
                        </td>
                    </tr>
                    <?php endif; ?>

                    <?php

                        // recuperando lancamentos cadastrados para os niveis
                        $arrLancamentos = $oLancamento->pegarLancamentosPorNivel($nivel['nvcid'], $entid);
                        
                        foreach($arrLancamentos as $lancamento):

                    ?>

                        <tr>
                            <td> <?php echo $lancamento[ "cpgid" ] . " - " . $lancamento[ "cpgdsc" ]; ?> </td>
                            <td align="center"> <?php echo $oLancamento->pegarValorLancamento('2008',  $lancamento['cpgid']); ?> </td>
                            <td align="center"> <?php echo $oLancamento->pegarValorLancamento('2009',  $lancamento['cpgid']); ?> </td>
                            <td align="center"> <?php echo $oLancamento->pegarValorLancamento('2010',  $lancamento['cpgid']); ?> </td>
                            <td align="center"> <?php echo $oLancamento->pegarValorLancamento('2011',  $lancamento['cpgid']); ?> </td>
                            <td align="center"> <?php echo $oLancamento->pegarValorLancamento('2012',  $lancamento['cpgid']); ?> </td>
                            <td align="center">
                                <?php if( ($oLiberacao->noPrazo() || $oLiberacaoIndividual->noPrazo( $entid )) && ( $db->testa_superuser() || in_array(PERFIL_IFES_CADASTRO_PROGRAMA, pegaArrayPerfil($_SESSION['usucpf']) ) ) ): ?>
                                <a href="javascript: editar(<?php echo $lancamento['cpgid']; ?>);" title="Incluir" style="border: none;">
                                    <img src="../imagens/alterar.gif" border="0" align="absmiddle" alt="" />
                                </a>
                                <a href="javascript: excluir(<?php echo $lancamento['cpgid']; ?>);" title="Incluir" style="border: none;">
                                    <img src="../imagens/excluir.gif"  border="0" align="absmiddle" alt="" />
                                </a>
                                <?php endif; ?>
                            </td>
                        </tr>

                    <?php endforeach; ?>

                    <tr class="verde">
                        <td style="text-align: right !important"> Lan&ccedil;ado: </td>
                        <td> <?php echo $valorTotal2008 = $oLancamento->pegarValorLancamentoTotal('2008', $nivel['nvcid'], $entid); ?> </td>
                        <td> <?php echo $valorTotal2009 = $oLancamento->pegarValorLancamentoTotal('2009', $nivel['nvcid'], $entid); ?> </td>
                        <td> <?php echo $valorTotal2010 = $oLancamento->pegarValorLancamentoTotal('2010', $nivel['nvcid'], $entid); ?> </td>
                        <td> <?php echo $valorTotal2011 = $oLancamento->pegarValorLancamentoTotal('2011', $nivel['nvcid'], $entid); ?> </td>
                        <td> <?php echo $valorTotal2012 = $oLancamento->pegarValorLancamentoTotal('2012', $nivel['nvcid'], $entid); ?> </td>
                        <td> </td>
                    </tr>
                    <tr class="vermelho">
                        <td style="text-align: right !important"> Dispon&iacute;vel: </td>
                        <td> <?php echo $oLancamento->pegarValorLancamentoDisponivel('2008', $nivel['nvcid'], $valorTotal2008, $entid); ?> </td>
                        <td> <?php echo $oLancamento->pegarValorLancamentoDisponivel('2009', $nivel['nvcid'], $valorTotal2009, $entid); ?> </td>
                        <td> <?php echo $oLancamento->pegarValorLancamentoDisponivel('2010', $nivel['nvcid'], $valorTotal2010, $entid); ?> </td>
                        <td> <?php echo $oLancamento->pegarValorLancamentoDisponivel('2011', $nivel['nvcid'], $valorTotal2011, $entid); ?> </td>
                        <td> <?php echo $oLancamento->pegarValorLancamentoDisponivel('2012', $nivel['nvcid'], $valorTotal2012, $entid); ?> </td>
                        <td> </td>
                    </tr>
                </tbody>
            </table>

            </div>
        </div>

    <?php endforeach;?>

</form>
