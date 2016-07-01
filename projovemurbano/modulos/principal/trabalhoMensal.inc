<?php
if($_SESSION['projovemurbano']['ppuid']==3){
		$filtrotprid = "AND tprid='{$_SESSION['projovemurbano']['tprid']}'";
	}
$sql = "SELECT pmupossuipolo 
        FROM projovemurbano.polomunicipio 
        WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' 
        AND pmustatus='A'
        $filtrotprid";
$polomunicipio = $db->pegaLinha($sql);


$nucid = 0;
$perId = 0;
$turId = 0;

if( isset( $_GET['perid'] ) )
    $perId = (int) $_GET['perid'];

if( isset( $_GET['turid'] ) )
    $turId = (int) $_GET['turid'];

if( isset( $_GET['nucid'] ) )
    $nucid = (int) $_GET['nucid'];
?>

<style type="text/css" >
    #dialogAjax{
        background-color:#ffffff;
        position:absolute;
        color:#000033;
        top:50%;
        left:40%;
        border:2px solid #cccccc;
        width:20%;
        font-size:12px;
        padding: 20px;
        line-height: 20px;
        display: none;
    }
</style>
    
<form name="frmDiarioFrequencia" id="frmDiarioFrequencia" method="get">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
            <td class="SubtituloDireita" width="25%">N&uacute;cleo</td>
            <td id="container-nucleo" >
                <?php
                $nucleos = pegarNucleos( $polomunicipio['pmupossuipolo'] );
                if($nucleos == false) {
                    echo 'Não foram encontrados núcleos';
                } else {
                    $db->monta_combo('nucid', $nucleos, 'S', 'Selecione', '', '', '', '', 'S', 'nucid');
                }
                ?>
            </td>
		</tr>
		<tr>
            <td class="SubtituloDireita" width="25%">Turma</td>
            <td id="container-turma">
                <?php
                
                if( $turId == 0 )
                {
                    $db->monta_combo('turid', array(), 'S', 'Selecione um núcleo', '', '', '', '', 'N', 'turid', '');
                }
                else
                {
                    buscarTurmasComAlunos( array( 'nucid' => $nucid, 'turid' => $turId ) );
                }
                
                ?>
            </td>
		</tr>
		<tr>
            <td class="SubtituloDireita" width="25%"></td>
            <td id="container-diario">
                <?php
                
                if( $perId == 0 )
                {
                    $db->monta_combo('perid', array(), 'S', 'Selecione uma turma', '', '', '', '', 'N', 'perid', '');
                }
                else
                {
                    buscarPeriodoDiario( array( 'turid' => $turId, 'perid' => $perId ) );
                }
                ?>
            </td>
		</tr>
		<tr>
            <td class="SubtituloDireita" width="25%">&nbsp;</td>
            <td>
                <input type="button" name="btnVisualizarTrabalho" id="btnVisualizarDiario" value="Visualizar Trabalho(s)" />
                <span id="msg"></span>
            </td>
		</tr>
        <tr>
            <td colspan="2">
                <div id="container-trabalho-mensal"></div>
            </td>
        </tr>
    </table>
    <input type="hidden" value="<?php echo $perId; ?>" name="perid_freq" id="perid_freq" />
    <input type="hidden" value="<?php echo $turId; ?>" name="tur_freq" id="turid_freq" />
</form>

<div id="dialogAjax" >
    <img src="../imagens/wait.gif" alt="ajax"/>Aguarde! Carregando Dados...
</div>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-1.5.1.min.js"></script>
<script language="javascript" type="text/javascript" src="../projovemurbano/js/trabalho_mensal.js"></script>
<script type="text/javascript" language="javascript" >
    $(document).ready(function(){
        TrabalhoMensal.init();
    });
</script>