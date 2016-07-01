<?
include_once '_funcoessubacoes.php';
include APPRAIZ ."includes/workflow.php";
require APPRAIZ . 'includes/library/simec/Listagem.php';


if($_REQUEST['requisicao']) {    
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

if(!isset($_SESSION['planacomorc']['id_subacao'])){
 echo "
    <script>
        alert('A sessão foi perdida, acesse a página novamente.');
        window.location.href = 'planacomorc.php?modulo=inicio&acao=C';                
    </script>";
 die();
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

$sql = "SELECT * FROM planacomorc.subacao WHERE id_subacao='" . $_SESSION['planacomorc']['id_subacao'] . "'";
$subacao = $db->pegaLinha($sql);
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<section class="col-md-12">
    <ol class="breadcrumb">
	<li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?=$_SESSION['sisdsc']; ?></a></li>
	<li class="">Subações <?= $_SESSION['exercicio']; ?></li>
	<li class="active"><?='<strong>Subação</strong>: '. $subacao['codigo'] . ' - ' . $subacao['titulo'] ?></li>
    </ol>
    <?php 
    montaAbasSisopSubAcoes(preg_replace('@&id_.*@i', '', $_SERVER['REQUEST_URI'])); 
    ?>
</section>

<?
if($_REQUEST['aba']) include_once APPRAIZ_SISOP.'subacoes/'.$_REQUEST['aba'].'.inc';
?>
