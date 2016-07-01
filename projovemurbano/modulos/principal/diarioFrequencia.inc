<?php
if($_SESSION['projovemurbano']['ppuid']==3){
		$filtrotprid = "AND tprid='{$_SESSION['projovemurbano']['tprid']}'";
	}
$sql = "SELECT * 
        FROM projovemurbano.polomunicipio 
        WHERE 
			pjuid='".$_SESSION['projovemurbano']['pjuid']."' 
        AND pmustatus='A'
		$filtrotprid";
$polomunicipio = $db->pegaLinha($sql);
$perfis = pegaPerfilGeral();
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
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<form name="frmDiarioFrequencia" id="frmDiarioFrequencia" method="post" action="">
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
                $db->monta_combo('turid', array(), 'S', 'Selecione', '', '', '', '', 'S', 'turid', '');
                ?>
            </td>
		</tr>
		<tr>
            <td class="SubtituloDireita" width="25%">Componente Curricular</td>
			<td id="container-componente-curricular">
                <?php
                $sql = "SELECT coc.cocid as codigo, coc.cocnome as descricao
                        FROM projovemurbano.componentecurricular coc
                        INNER JOIN projovemurbano.gradecurricular grd
                            ON coc.cocid = grd.cocid
                        INNER JOIN projovemurbano.programaprojovemurbano ppu
                            ON grd.ppuid = ppu.ppuid
                        WHERE coc.cocstatus = 'A'
                        AND ppu.ppustatus   = 'A'
                        AND ppu.ppuid = ". PROJOVEMURBANO_2012 ."
                        AND coc.cocdisciplina = 'D'
                        ORDER BY coc.cocnumordem";

               	$dadosComponente = $db->carregar($sql);
				$dadosComponente[] = array(
					'codigo'	=> '9999',
					'descricao'	=> 'Todos'
				);
				
                $db->monta_combo('cocid', $dadosComponente, 'S', 'Selecione', '', '', '', '', 'S', 'cocid', '');

                   
                ?>
            </td>
            
            
		</tr>
                
                <tr>
            <td class="SubtituloDireita" width="25%"></td>
			<td id="container-componente-periodo">
                <?
                if($_SESSION['projovemurbano']['ppuid'] !=3){
                	$dataRange = "			|| ' - '
				|| to_char(perdtinicio,'DD/MM/YYYY')
				|| ' a '
				|| to_char(perdtfim,'DD/MM/YYYY')";
                }
                $sqlPERIODOS = "SELECT
        							perid as codigo,
        							perdesc $dataRange as descricao
                    			FROM 
        							projovemurbano.periodocurso where
                    				perstatus = 'A'   
			                    AND   ppuid = ".$_SESSION['projovemurbano']['ppuid']."
			                    --AND perdtinicio <= '".date('Y-m-d')."'
			        			AND perid!=37
			                    order by 
        							perid asc";
                
				$dados = $db->carregar($sqlPERIODOS);
			
				$db->monta_combo('perid', $sqlPERIODOS, 'S', 'Selecione', '', '', '', '', 'S', 'perid');
                ?>
            </td>
		</tr>
		<tr>
            <td class="SubtituloDireita" width="25%">&nbsp;</td>
			<td>
                <input type="button" name="btnVisualizarDiario" id="btnVisualizarDiario" value="Visualizar Diário" />
                &nbsp;
                &nbsp;
                <?php if(!in_array( PFL_CONSULTA, $perfis )) { ?>
                <input type="button" name="btnGerarDiario" id="btnGerarDiario" value="Gerar Diário" />
                <?php }?>
                
            </td>
		</tr>
        <tr>
            <td colspan="2">
                <div id="container-diario"></div>
            </td>
        </tr>
    </table>
    <input type="hidden" name="ppuid" id="ppuid" value="<?php echo PROJOVEMURBANO_2012; ?>" />
</form>

<div id="dialogAjax" >
    <img src="../imagens/wait.gif" alt="ajax"/>Aguarde! Carregando Dados...
</div>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-1.5.1.min.js"></script>
<script language="javascript" type="text/javascript" src="../projovemurbano/js/diario_frequencia.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>

<script type="text/javascript" language="javascript" >
    $(document).ready(function(){
        
        $("#frmDiarioFrequencia").validate({
            //Define as regras dos campos
            rules:{
                    nucid : {required: true},
                    turid : {required: true},
                    perid : {required: true},
                    cocid : {required: true}
                  
              
            },
            //Define as mensagesn de alerta
            messages:{
                    nucid : 'Campo Obrigatório',
                    turid : 'Campo Obrigatório',
                    perid : 'Campo Obrigatório',                    
                    cocid : 'Campo Obrigatório'
            }
        });
    
        DiarioFrequencia.init();
    });
    
    

</script>