<link rel="stylesheet" type="text/css" href="./css/geral.css" />
<form name="frmDiarioFrequenciaMensal" id="frmDiarioFrequenciaMensal" method="get">
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
                    $db->monta_combo('turid', array(), 'S', 'Selecione uma Escola', '', '', '', '', 'S', 'turid', '');
                ?>
            </td>
		</tr>
		<tr>
            <td class="SubtituloDireita" width="25%">Período</td>
            <td id="container-diario">
                <?php
                    $db->monta_combo('perid', array(), 'S', 'Selecione uma turma', '', '', '', '', 'S', 'perid', '');
                ?>
            </td>
		</tr>
		<tr>
            <td class="SubtituloDireita" width="25%">&nbsp;</td>
	    <td>
                <input type="button" name="btnVisualizarDiario" id="btnVisualizarDiario" value="Visualizar Diário" />
                <span id="msg"></span>
            </td>
		</tr>
        <tr>
            <td colspan="2">
                <div id="container-diario-frequencia-mensal"></div>
            </td>
        </tr>
    </table>
    
</form>
<div id="dialogAjax" >
    <img src="../imagens/wait.gif" alt="ajax"/>Aguarde! Carregando Dados...
</div>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-1.5.1.min.js"></script>
<script language="javascript" type="text/javascript" src="../projovemcampo/js/frequencia_mensal.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" language="javascript" >
    $(document).ready(function(){
    	
        $("#frmDiarioFrequenciaMensal").validate({

            //Define as regras dos campos
            rules:{
                    perid : {required: true},
                    turid : {required: true},
                    entid : {required: true}
            },
            //Define as mensagens de alerta
            messages:{
                    perid : 'Campo Obrigatório',
                    turid : 'Campo Obrigatório',
                    entid : 'Campo Obrigatório'
            }
        });
    	$('[class*="somaaulasdadas"]').live('blur',function(){

    		var codigoensino = $(this).attr('codigoensino');
     		var qtdE = 0;
    		var qtdC = 0;	
    		var somapresenca = 0;
    		var qtd = 0;
    		
    		if($('#qtdaulasdadas_E').val()!=''){
        			qtdE = $('#qtdaulasdadas_E').val();
    		}
    		if($('#qtdaulasdadas_C').val()!=''){
        			qtdC = $('#qtdaulasdadas_C').val();
    		}
    		if($('#qtdaulasdadas_'+codigoensino).val()!=''){
    			qtd = $('#qtdaulasdadas_'+codigoensino).val();
			}
//     		var alert1 = '';
    		$('[class*="qtdaulas_'+codigoensino+'"]').each(function(){
    			var valor 	= $(this).val();
             	if( parseInt(valor) > parseInt(qtd) ){
                     $(this).val('0');
//                      alert1 = 'oi';
				}
            });
//             if(alert!=''){
//     			alert(alert1);
//             }
    		somapresenca = parseInt(qtdE)  + parseInt(qtdC);
    		
//     		if(somapresenca > 0){
    			$('#totalaulasdadas').html(somapresenca);
//     		}else{
//     			$('#somaQuantidadePresenca_'+idestudante +'').html(0);
//     		}
    		
    	});
    	
    	$('[class_2="somaaulasfrequentadas"]').live('blur',function(){
    		
    		var idestudante = $(this).attr('idestudante');
    		
     		var qtdE = 0;
    		var qtdC = 0;	
    		var somapresenca = 0;
    		
    		if($('#'+idestudante+'_E').val()!=''){
        			qtdE = $('#'+idestudante+'_E').val();
    		}
    		if($('#'+idestudante+'_C').val()!=''){
        			qtdC = $('#'+idestudante+'_C').val();
    		}
    		if(parseInt(qtdE)>parseInt($('#qtdaulasdadas_E').val())){
    			alert('A quantidade de aulas frequentadas para Tempo Escola não pode ser maior que a quantidade de aulas dadas.');
    			$('#'+idestudante+'_E').val('0');
    			qtdE = 0;
    		}
    		if(parseInt(qtdC)>parseInt($('#qtdaulasdadas_C').val())){
    			alert('A quantidade de aulas frequentadas para Tempo Comunidade não pode ser maior que a quantidade de aulas dadas.');
    			$('#'+idestudante+'_C').val('0');
    			qtdC = 0;
    		}
    		somapresenca = parseInt(qtdE)  + parseInt(qtdC);
    		
//     		if(somapresenca > 0){
    			$('#somaQuantidadePresenca_'+idestudante +'').html(somapresenca);
//     		}else{
//     			$('#somaQuantidadePresenca_'+idestudante +'').html(0);
//     		}
    		
    	});
		
        DiarioFrequenciaMensal.init();
    });
</script>
