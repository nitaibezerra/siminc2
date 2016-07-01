<?php

// cabecalho da app
require_once APPRAIZ . "includes/cabecalho.inc";
require_once APPRAIZ . "academico/classes/CursoPosGraduacao.class.inc";
require_once APPRAIZ . "academico/classes/NivelCurso.class.inc";
echo '<br/>';

$abacod_tela = ($_SESSION['baselogin'] == "simec_desenvolvimento") ? ABA_CURSO_POSGRADUACAO_DEV : ABA_CURSO_POSGRADUACAO_OLD; //Dados Desenv

$db->cria_aba( $abacod_tela , $url, array() );
monta_titulo("Lista de Programas", "");

$oCursoPosGraduacao = new CursoPosGraduacao();
$oNivelCurso = new NivelCurso();

// recupera os niveis de curso cadastrados
$niveisCurso = $oNivelCurso->lista();

// extraindo dados para a tela
if($_POST)
    extract($_POST);
else
    extract($_GET);

$entid = $entid ? $entid : $_SESSION['academico']['entid'];

// cria o cabeçalho padrão do módulo
$autoriazacaoconcursos = new autoriazacaoconcursos();
echo $autoriazacaoconcursos->cabecalho_entidade($entid);

// se for para mostrar todos os registros
if(isset($_POST['btnTodos']) || (!isset($_POST['btnTodos']) && !isset($_POST['btnPesquisar']) && empty($_GET['nvcid']))){

    // reseta valores dos campos
    unset($cpgcodigo);
    unset($cpgdsc);
    $nvcid = $niveisCurso[0]['nvcid'];

    // monta a lista de niveis para selecao de todos os checkbox
    foreach($niveisCurso as $nivel){
        $arrNvcid[$nivel['nvcid']] = $nivel['nvcid'];
    }

    // grava na sessao a lista de niveis selecionados
    $_SESSION['arrNvcid'] = serialize($arrNvcid);

// se for para mostrar pelo filtro
}else{

    $nvcid = $_GET['nvcid'] ? $_GET['nvcid'] : $nvcid;

    // senao tiver acionado uma aba
    if(empty($nvcid)){

        // se selecionado 1 ou mais niveis
        if(is_array($arrNvcid)){

            // ativa a primeira aba do filtro
            $nvcid = $arrNvcid[0];

        }else{

            // ativa o primeiro nivel do banco como default
            $nvcid = $niveisCurso[0]['nvcid'];
            $arrNvcid = array($nvcid);
            
        }

        // grava na sessao a lista de niveis selecionados
        $_SESSION['arrNvcid'] = serialize($arrNvcid);

    }else{
    
        // se ativado uma aba, recupera os niveis selecionados da sessao
        $arrNvcid = unserialize($_SESSION['arrNvcid']);
        
    }

}

if($_GET['excluir'] == 'ok'){

    $oCursoPosGraduacao->excluir($cpgid);
    
}

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

    /**
     * Abre o popup para insercao do novo curso
     */
    function inserirNovoCurso(){

        window.open('?modulo=principal/cadCursoPosGraduacao&acao=A&entid=<?php echo $entid; ?>', 'cadCursoPosGraduacao', 'top=350, left=100, align=center,toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=850,height=160');
        
    }

    /**
     * Exclui o curso da listagem
     * @name editarCurso
     * @author Silas Matheus
     * @access public
     * @return void
     */
    function editarCurso(cpgid){

        window.open('?modulo=principal/cadCursoPosGraduacao&acao=A&entid=<?php echo $entid; ?>&cpgid=' + cpgid, 'cadCursoPosGraduacao', 'top=350, left=100, align=center,toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=850,height=160');
        
    }

    /**
     * Exclui o curso da listagem
     * @name excluirCurso
     * @author Silas Matheus
     * @param int cpgid - Código do curso
     * @access public
     * @return void
     */
    function excluirCurso(cpgid){
        <?php if(!in_array(570, pegaArrayPerfil($_SESSION['usucpf']))): ?>
            if(confirm('Deseja realmente excluir este Curso ?'))
                window.location = '?modulo=principal/pesquisaCursoPosGraduacao&acao=A&excluir=ok&entid=<?php echo $entid; ?>&cpgcodigo=<?php echo $cpgcodigo; ?>&cpgdsc=<?php echo $cpgdsc; ?>&nvcid=<?php echo $nvcid; ?>&cpgid=' + cpgid;
        <?php else: ?>
            alert('Seu perfil não possui permissão para exclusão de curso.')
        <?php endif; ?>
    }

</script>

<form id="formulario" name="formulario" action="?modulo=principal/pesquisaCursoPosGraduacao&acao=A" method="post" enctype="multipart/form-data" >

    <table id="tblCurso" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="SubTituloDireita" style="width:250px;"> C&oacute;digo: </td>
            <td> <?php echo campo_texto('cpgcodigo', 'S', 'S', '', 15, 15, '', '', '', '', '', 'id="cpgcodigo"'); ?> </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Nome: </td>
            <td> <?php echo campo_texto('cpgdsc', 'S', 'S', '', 80, 100, '', '', '', '', '', 'id="cpgdsc"'); ?> </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> N&iacute;vel: </td>
            <td>

                <?php

                foreach($niveisCurso as $nivel):

                    
                    if(is_array($arrNvcid) && in_array($nivel['nvcid'], $arrNvcid)){

                        $checked = "'checked='checked'";

                    }else{

                        $checked = "";

                    }

                ?>

                    <input type="checkbox" name="arrNvcid[]" id="nvcid<?php echo $nivel['nvcid']; ?>" value="<?php echo $nivel['nvcid']; ?>" <?php echo $checked; ?> />
                    <label for="nvcid<?php echo $nivel['nvcid']; ?>"> <?php echo $nivel['nvcdsc']; ?> &nbsp; </label>

                <?php endforeach; ?>
                

            </td>
        </tr>
        <tr>
            <th colspan="2" style="text-align: center;">
                <input type="hidden" value="<?php echo $entid; ?>" name="entid" id="entid" />
                <?php if( $db->testa_superuser() ): ?>
                <input type="button" value="Inserir Novo Programa" name="btnInserir" id="btnInserir" onclick="inserirNovoCurso()" />
                <?php endif; ?>
                <input type="submit" value="Pesquisar" name="btnPesquisar" id="btnPesquisar" />
                <input type="submit" value="Todos" name="btnTodos" id="btnTodos" />
            </th>
        </tr>
    </table>

</form>

<br />

<?php

// montando array de abas
$abasLista = array();

foreach($niveisCurso as $nivel):

    if(is_array($arrNvcid) && in_array($nivel['nvcid'], $arrNvcid)){
    array_push($abasLista, array(
        "id" => $nivel['nvcid'],
        "descricao" => $nivel['nvcdsc'],
        "link" => "?modulo=principal/pesquisaCursoPosGraduacao&acao=A&entid=$entid&cpgcodigo=$cpgcodigo&cpgdsc=$cpgdsc&nvcid={$nivel['nvcid']}"));
    }

endforeach;
echo montarAbasArray($abasLista, "?modulo=principal/pesquisaCursoPosGraduacao&acao=A&entid=$entid&cpgcodigo=$cpgcodigo&cpgdsc=$cpgdsc&nvcid=$nvcid");

// titulo da listagem
monta_titulo("Lista de Cursos", "");
    
// retorna todos os dados para a primeira aba
$oCursoPosGraduacao->montaGrid($entid, $nvcid, $cpgcodigo, $cpgdsc);

if( $db->testa_superuser() ){ ?>
<script>
	$("img[title=Alterar]").parent('a').show();
	$("img[title=Excluir]").parent('a').show();
</script>
<?php }else if( in_array(PERFIL_IFES_CADASTRO_PROGRAMA, pegaArrayPerfil($_SESSION['usucpf']) )){ ?>
<script>
	$("img[title=Alterar]").parent('a').show();
	$("img[title=Excluir]").parent('a').hide();
</script>
<?php }else{ ?>
<script>
	$("img[title=Alterar]").parent('a').hide();
	$("img[title=Excluir]").parent('a').hide();
</script>
<?php } ?>