<?php
if($_REQUEST['arquivo']){
         include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
                    $file = new FilesSimec(null, null, "cproc");
                    $file->getDownloadArquivo($_REQUEST['arquivo']);
}

if($_REQUEST['excluir'] && $_REQUEST['mntid']){

    $arqid = $db->pegaUm("select arqid from cproc.minutaarquivo where mntid = ".$_REQUEST['mntid']);

    $sql = "delete from cproc.minutaarquivo where mntid = ".$_REQUEST['mntid'];
    $db->executar($sql);

    $sql = "delete from cproc.minuta where mntid = ".$_REQUEST['mntid'];
    $db->executar($sql);

    $db->commit();

    $caminho = APPRAIZ."arquivos/cproc".'/'.floor($arqid/1000).'/'.$arqid;
    if(file_exists($caminho)){
        unlink($caminho);
    }

    echo "<script>alert('Minuta excluida com sucesso!');</script>";
    echo "<script>location.href = 'cproc.php?modulo=principal/minuta&acao=A';</script>";

    die;

}

include  APPRAIZ."includes/cabecalho.inc";

$titulo_modulo = "Minutas";
$modulo = "";
monta_titulo( $titulo_modulo, $modulo );

// dependências --------------------------------------------------------------------------------------------------
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . "cproc/classes/ModeloCproc.class.inc";
include_once APPRAIZ . "cproc/classes/Processo.class.inc";
include_once APPRAIZ . "cproc/classes/WorkflowCproc.class.inc";
// --------------------------------------------------------------------------------------------------
include_once '../../cproc/classes/PenalidadeAplicada.class.inc';

if( !verificaExistenciaPrcid() ){
	echo "
		<script>
			alert('Sessão perdeu processo.');
			window.location.href = 'cproc.php?modulo=principal/lista_grid_processo&acao=A';
		</script>";
	exit;	
}
 if ($_SESSION['cproc']['prcid']) {
         $processo = exibirProcesso($_SESSION['cproc']['prcid']);
         extract($processo);
    }
    else{
        $processo = exibirProcesso($prcid);
    }


$sql = " 
    SELECT COUNT(*) as total
    FROM cproc.processoanexado
    WHERE pro_prcid = {$_SESSION['cproc']['prcid']}
";
$processosAnexados = $db->carregar( $sql );
if( $processosAnexados[0]['total'] > 0 ){
    echo "<script>
        alert('Esse processo não pode receber minuta por ser um processo anexado!');
        window.history.back();
    </script>";
    exit;
}


?>

<br>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript">
	jQuery.noConflict();
	
	jQuery('.adicionar').live('click',function(){
		return window.open('assint.php?modulo=principal/cadCursoPEC_G&acao=A','modelo',"height=450,width=850,scrollbars=yes,top=50,left=200");
	});
</script>

<?php 
$arMnuid = array();
$db->cria_aba( ABA_CAD_PROCESSO, $url, '', $arMnuid );

$sql = "select * from  cproc.minuta min"
        . " join cproc.minutaarquivo mia on min.mntid = mia.mntid"
        . " join cproc.tipodocprocesso mtp on mtp.tdpid = min.tdpid  "
        . " left join seguranca.usuario usu on min.usucpf = usu.usucpf "
        . " where min.prcid = ".$prcid ;

$listaArquivos = $db->carregar($sql);

?>

<div style="width:98%;margin:0 auto;">
	
<?php if (!empty($listaArquivos)){?>
	<div>
		<h4>Lista de minutas:</h4>

                <table>
                    <tr><td class="subtituloCentro">Download da minuta</td>
                        <td class="subtituloCentro">Tipo da minuta</td>
                        <td class="subtituloCentro">Data inclusão</td>
                        <td class="subtituloCentro">Hora inclusão</td>
                        <td class="subtituloCentro">Responsável</td>
                    </tr>
                
                    
                    <?php foreach ($listaArquivos as $value) { 
                        
                        
                        ?>
                    <tr>
                        <td width="250" style="text-align: center">
                            <?
                            $pfls = arrayPerfil();

                            $esdid = $db->pegaUm("select esdid from cproc.processo p
                                                  inner join workflow.documento d on d.docid = p.docid
                                                  where p.prcid = ".$_SESSION['cproc']['prcid']);
                            if($esdid == "1312"){
                                if (in_array(PERFIL_SUPER_USUARIO, $pfls) || $_SESSION['usucpf'] == $value[usucpf] ){
                                ?>
                                <img src="../imagens/excluir.gif" class="excluir" onclick="excluirMinuta('<?=$value[mntid];?>');" style="cursor:pointer;">
                             <?}
                            }?>
                            &nbsp;
                            <a href="?modulo=principal/minuta&acao=A&arquivo=<?php echo $value[arqid];?>">Download</a>
                        </td>
                        <td width="250" style="text-align: center"><?php echo $value[tdpdsc]; ?></td>
                        <td width="250" style="text-align: center"><?php echo date('d/m/Y', strtotime($value[mntdatainclusao]));?></td>
                        <td width="250" style="text-align: center"><?php echo date('H:i', strtotime($value[mntdatainclusao]));?></td>
                        <td width="250" style="text-align: center"><?php echo $value[usunome];?></td>
                    </tr>
                    <?php } ?>
                    
                </table>
        </div>
<?php } else{
    
    echo "<div style='padding:20px;'> nenhuma minuta cadastrada nesse processo! </div>";
    
}?>
		

</div>

<script>

function excluirMinuta( chave ){
    if( confirm('Deseja apagar esse item?') ){
        window.location.href = "cproc.php?modulo=principal/minuta&acao=A&excluir=1&mntid="+chave;
    }
}

function salvar(){
	document.forms.formPenalidadeAplicada.submit();
}

function editar( chave ){
	window.location.href = "cproc.php?modulo=principal/penalidadeaplicada&acao=A&papid="+chave;
}

function excluir( chave ){
	if( confirm('Deseja apagar esse item?') ){
		window.location.href = "cproc.php?modulo=principal/penalidadeaplicada&acao=A&excluir=1&papid="+chave;
	}
}


</script>