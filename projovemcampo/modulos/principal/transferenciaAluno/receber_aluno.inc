<?php
    monta_titulo('','Dados da Transferência');
    
    $bloq = "S";
    $bloq = 'S'
?>
<?
   
?>
<style type="">
.ui-dialog-titlebar{
text-align: center;
}
</style>
<script language="javascript" type="text/javascript">
    jQuery(document).ready(function(){  
          
        jQuery('#formTranferencia').children(':first').before('<input type="hidden" name="requisicao" id="requisicao" value=""/>');
          
        jQuery('.chk').live('click',function(){
            var traid = jQuery(this).val();
            if( jQuery(this).attr('checked') ){
                var entid = jQuery(this).attr('id');
                jQuery.ajax({
                    type: "POST",
                    url: window.location.href,
                    data: "requisicao=buscarTurmasDestino&entid=" + entid + "&bloq=<?=$bloq ?>",
                    async: false,
                    success: function(msg){
                        jQuery('#div_turma_'+traid).html(msg);
                        jQuery('#div_turma_'+traid).children(':first').attr('id','turid_'+traid);
                        jQuery('#div_turma_'+traid).children(':first').attr('name','turid['+traid+']');
                    }
                });
            }else{
                jQuery('#div_turma_'+traid).html(' ');
            }
        });
        
        jQuery('.tranferir').click(function(){
            // Valida campos obrigatórios
            if( jQuery('.chk:checked').length < 1 ){
                alert('Selecione pelo menos 1 aluno');
                return false;
            }
            var erro = false;
            jQuery('.obrigatorio').each(function(){
                if( jQuery(this).val() == '' ){
                    alert('Campo obrigatório.');
                    jQuery(this).focus();
                    erro = true;
                    return false;
                }
            });
            if( erro ){
                return false;
            }
            //FIM Valida campos obrigatórios
            jQuery('#requisicao').val('receber_aluno_tranferir');
            jQuery('#formTranferencia').submit();
        });
        
        jQuery('.cancelar').click(function(){
            if( jQuery('#tr_justificativa').css('display') == 'none' ){
                jQuery('#tr_justificativa').show();
                alert('Preencha a justificativa.');
                return false;
            }else{
                // Valida campos obrigatórios
                if( jQuery('.chk:checked').length < 1 ){
                    alert('Selecione pelo menos 1 aluno');
                    return false;
                }
                if( jQuery('#justificativa').val() == '' ){
                    alert('Preencha a justificativa.');
                    jQuery('#justificativa').focus();
                    return false;
                }
                jQuery('#formTranferencia').children(':first').before('<input type="hidden" name="justificativa" value="'+jQuery('#justificativa').val()+'"/>');
                //FIM Valida campos obrigatórios
                jQuery('#requisicao').val('receber_aluno_cancelar');
                jQuery('#formTranferencia').submit();
            }
        });
        
        jQuery('.detalhe').live('click',function(){
            var traid = jQuery(this).attr('id');
            jQuery.ajax({
                type: "POST",
                url: window.location.href,
                data: "&requisicao=mostraDetalheTransferencia&traid="+traid,
                async: false,
                success: function(msg){
                    jQuery( "#dialog-detalhe" ).html(msg);       
                    jQuery( "#dialog-detalhe" ).dialog({
                        resizable: false,
                        height:500,
                        width:800,
                        modal: true,
                        show: { effect: 'drop', direction: "up" },
                        buttons: {
                            "OK": function() {
                                jQuery( this ).dialog( "close" );                            
                            }
                        }
                    });
                }
            });
        });
        
        jQuery('.baixar').click(function(){
            jQuery('#formTranferencia').children(':first').before('<input type="hidden" name="arqid" value="'+jQuery(this).attr('id')+'"/>');
            jQuery('#formTranferencia').submit();
        });
    });
</script>	
<?

monta_titulo('Alunos a transferir','');

$sqlano = " SELECT
                ppuano as ano2
             FROM
                projovemurbano.programaprojovemurbano ppu
            WHERE ";

$sql = "SELECT 
			'<center><input type=checkbox name=traid[] value='|| tra.traid ||' id='|| tra.entid_destino ||' class=chk />
                     <div id=div_turma_'|| tra.traid ||' ></div>
            </center>' as marcar, 
			shtdescricao as status,
			est.estcpf as cpf,
			est.estnome as nome,
			CASE 
				WHEN apcO.apcesfera = 'E'
				THEN munO.estuf 
				ELSE munO.estuf||' - '||munO.mundescricao
			END as localOrigem,
			aprO.apranoreferencia as anoOrigem,
			CASE 
				WHEN apcO.apcesfera = 'E'
				THEN 'Estadual'
				ELSE 'Municipal'
			END as esfera,
		aprD.apranoreferencia as anoDestino,
			CASE 
				WHEN arqt.arqid IS NULL
				THEN 'Não Existente'
				ELSE '<center><a id='||arqt.arqid||' style=\"cursor:pointer;color:blue\" class=baixar>Baixar</a></center>'
			END as arquivo
		FROM 
		    projovemcampo.transferencia tra 
		INNER JOIN projovemcampo.estudante			est  ON est.estid  = tra.estid 
		LEFT  JOIN projovemcampo.arquivo_tranferencia		arqt ON arqt.traid = tra.traid
		INNER JOIN projovemcampo.historico_transferencia	hst  ON hst.htrid  = tra.htrid_ultimo
		INNER JOIN projovemcampo.statushistorictransferencia	sht  ON sht.shtid  = hst.shtid_status
		
		INNER JOIN projovemcampo.turma turO ON turO.turid = tra.turid_origem
		INNER JOIN projovemcampo.adesaoprojovemcampo apcO ON apcO.secaid = turO.secaid
		INNER JOIN projovemcampo.anoprograma aprO ON aprO.aprid = apcO.aprid
		INNER JOIN entidade.entidade entO ON entO.entid = turO.entid
		INNER JOIN entidade.endereco endeO ON endeO.entid = entO.entid
		INNER JOIN territorios.municipio munO ON munO.muncod = endeO.muncod
		--Dados Destino 
		INNER JOIN projovemcampo.turma turD ON turD.entid = tra.entid_destino
		INNER JOIN projovemcampo.adesaoprojovemcampo apcD ON apcD.secaid = turD.secaid
		INNER JOIN projovemcampo.anoprograma aprD ON aprD.aprid = apcD.aprid
		WHERE   
		    hst.shtid_status = 2 
		AND	apcD.apcid = {$_SESSION['projovemcampo']['apcid']} ";
// ver($sql);
$cabecalho = array("&nbsp","Status","CPF Aluno","Aluno","Local Origem","Ano Origem","Esfera Origem","Ano Destino","Arquivo Anexo", "Detalhe");
$db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', '', 'formTranferencia');
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="Center">
    <tr id="tr_justificativa" style="display:none">
        <td class="SubTituloDireita" width="25%" >Texto Adicional:</td>
        <td>
            <?=campo_textarea("justificativa","N","S","Digite o outro motivo", 60, 3, 200,"","","","","")?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloCentro">
      	<?if(!in_array ( PFL_CONSULTA, $perfis )
//       			$permiteTransferencia =='t'
			){?> 
            <input type="button" class="tranferir" value="Transferir">
            <input type="button" class="cancelar" value="Cancelar Transfência">
        <?}?>
        </td>
    </tr>
</table>

<div id="dialog-detalhe" title="Solicitações do Processo" style="display:none;">
</div>
