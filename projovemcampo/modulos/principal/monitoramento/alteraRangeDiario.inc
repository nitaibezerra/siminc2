<?php
// 	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	// monta cabeçalho
	
	
					
?>
		<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
		<script language="javascript" type="text/javascript">
	
			/**
			 * Submete o formulário
			 * @name enviaArquivos
			 * @return void
			 */
			function enviaArquivos( arquivo ){
			   var formulario = $('formulario');
			   meuerro = "";
			   if (!arquivo) {
			      //Se não tenho arquivo, é porque não foi selecionado um arquivo no formulário.
			        meuerro = "Por favor, selecionar o arquivo que deseja anexar para realizar essa ação.";
			   }
			   else{
		   		   	formulario.submit();
			   }
			   //se estou aqui é porque não se pode submeter
			   //alert (meuerro);
				   //return 0;
			}
				
			function excluirAnexo( arqid ){
			 	if ( confirm( 'Deseja excluir este Documento?' ) ) {
			 		location.href= window.location+'&arqidDel='+arqid;
			 	}
			}
		
		</script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<form name="frmalteraRangeDiario" id="frmalteraRangeDiario" method="post" action="">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
            <td class="SubtituloDireita" width="25%">Escola</td>
            <td id="container-escola" >
                <?php
                $escolas =  pegarEscolas(true);
                if($escolas == false) {
                    echo 'Não foram encontradas escolas';
                } else {
                    $db->monta_combo('entid', $escolas, 'S', 'Selecione', '', '', '', '', 'S', 'entid');
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
            <td class="SubtituloDireita" width="25%">Período</td>
			<td id="container-periodo">
                <?php
				$db->monta_combo('perid', array(), 'S', 'Selecione', '', '', '', '', 'S', 'perid');
                ?>
            </td>
		</tr>
		<tr>
            <td class="SubtituloDireita" width="25%">&nbsp;</td>
			<td>
                <input type="button" name="btnAlteraRange" id="btnAlteraRange" value="Alterar" />
            </td>
		</tr>
    </table>
</form>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-1.5.1.min.js"></script>
<script language="javascript" type="text/javascript" src="../projovemcampo/js/alteraRangeDiario.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>

<script type="text/javascript" language="javascript" >
    $(document).ready(function(){
        
        $("#frmalteraRangeDiario").validate({
            //Define as regras dos campos
            rules:{
                    entid : {required: true},
                    turid : {required: true},
                    perid : {required: true},
                  
              
            },
            //Define as mensagesn de alerta
            messages:{
                    entid : 'Campo Obrigatório',
                    turid : 'Campo Obrigatório',
                    perid : 'Campo Obrigatório'
            }
        });
        alteraRangeDiario.init();
    });
    
    

</script>