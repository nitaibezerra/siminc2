<?php

    if ($_POST['requisicao'] == 'salvarTermo'){
    	
    	extract($_POST);

    	if( $necessitasugestaoampliacao == 'sim'){
    		
    		if( $metasugerida > 0 ){
    			
    			$sql = "delete from projovemcampo.meta where tpmid in ( 2 ) and apcid=".$_SESSION['projovemcampo']['apcid'];
    			$db->executar($sql);

    			$sql = "insert into projovemcampo.meta (apcid, tpmid, metvalor ) values ( ".$_SESSION['projovemcampo']['apcid'].", 2, ".$metasugerida.");";
    			$db->executar($sql);
    			
    			$sql = "update projovemcampo.adesaoprojovemcampo set apcampliameta = 't' where apcid = ".$_SESSION['projovemcampo']['apcid'];
    			$db->executar($sql);
    			
    			$db->commit();
    			
    		}
    		if( $metaajustada > 0 ){
    			
    			$sql = "delete from projovemcampo.meta where tpmid in ( 3 ) and apcid=".$_SESSION['projovemcampo']['apcid'];
    			$db->executar($sql);
    		
    			$sql = "insert into projovemcampo.meta (apcid, tpmid, metvalor ) values ( ".$_SESSION['projovemcampo']['apcid'].", 3, ".$metaajustada.");";
    			$db->executar($sql);
    			
    			$sql = "update projovemcampo.adesaoprojovemcampo set apcampliameta = 't' where apcid = ".$_SESSION['projovemcampo']['apcid'];
    			$db->executar($sql);
    			
    			$db->commit();
    			$db->sucesso('principal/termoAdesaoAjustado', '');
    		}
    		$db->sucesso('principal/termoAdesao', '');
    		
    		die;
    		
    	}else{
    		
    		$sql = "delete from projovemcampo.meta where tpmid in ( 2, 3 ) and apcid=".$_SESSION['projovemcampo']['apcid'];
    		$db->executar($sql);
    		
    		$sql = "update projovemcampo.adesaoprojovemcampo set apcampliameta = 'f' where apcid = ".$_SESSION['projovemcampo']['apcid'];
    		$db->executar($sql);
    		 
    		$db->commit();
    		
    		$db->sucesso('principal/termoAdesao', '');
    		die;
    	}
    }

  

    include_once APPRAIZ . 'includes/cabecalho.inc';
    echo '<br>';

    echo montarAbasArray(montaMenuProjovemCampo(), $_SERVER['REQUEST_URI']);

    monta_titulo('Projovem Campo', montaTituloEstMun());

    $perfil = pegaPerfilGeral();
    
    if(!in_array(PFL_EQUIPE_MEC, $perfil) && !in_array(PFL_ADMINISTRADOR, $perfil) && !$db->testa_superuser()){
    	$bloqueioHorario = bloqueioadesao();
    }
    
    
//     if($bloqueioHorario ){
    	$habilita = 'N';
//     }else{
//     	$habilita = 'S';
//     }
    

	if( in_array(PFL_ADMINISTRADOR, $perfil) || in_array(PFL_SECRETARIO_MUNICIPAL, $perfil)  || in_array(PFL_SECRETARIO_ESTADUAL, $perfil) || in_array(PFL_EQUIPE_MEC, $perfil) || in_array(PFL_ADMINISTRADOR, $perfil)){
		$habilitaMetaSugerida = 'S';
	}else{
		$habilitaMetaSugerida = 'N';
   	}

	if( $db->testa_superuser() || in_array(PFL_EQUIPE_MEC, $perfil) || in_array(PFL_ADMINISTRADOR, $perfil) ){
		
		$habilitaMetaAjustada= 'S';
		$habilita = 'S';
	}


    
?>

<script language="javascript" type="text/javascript"
	src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
    
    //Faz a verificação do perfil e desabilita o radio button
    jQuery(document).ready(function() {
        
        if(parseInt($('[name=ppuid]').val()) === 2){
            $.ajax({
                type    : "POST",
                url     : window.location,
                data    : "requisicao=verificaMetaDestinada&suaid=<?=$sugestaoampliacao['suaid'];?>&metaDestinada=sugerida",
                async   : false,
                success: function(resp){
                    if(resp === '8'){
                        $('#metaDestinada_sugerida_J').attr('checked',true);
                    }else{
                        $('#metaDestinada_sugerida_P').attr('checked',true);
                    }
                }
            });

            $.ajax({
                type    : "POST",
                url     : window.location,
                data    : "requisicao=verificaMetaDestinada&suaid=<?=$sugestaoampliacao['suaid'];?>&metaDestinada=ajustada",
                async   : false,
                success: function(resp){
                    if(resp === '9'){
                        $('#metaDestinada_ajustada_J').attr('checked',true);
                    }else{
                        $('#metaDestinada_ajustada_P').attr('checked',true);
                    }
                }
            });
        }
        
        $('#necessitasugestaoampliacao').click(function() {
            $.ajax({
                type    : "POST",
                url     : window.location,
                data    : "requisicao=verificaMetaDestinada&cmeid=<?=$rsDadosMeta['cmeid'];?>&metaDestinada=atendida",
                async   : false,
                success: function(resp){
                    if(resp === '7'){
                        $('#metaDestinada_sugerida_J').attr('checked',true);
                    }else{
                        $('#metaDestinada_sugerida_P').attr('checked',true);
                    }
                }
            });
        });

        $('[name=necessitasugestaoampliacao]').click(function() {
            if (this.value == 'sim') {
                $('.tr_sugestao_ampliacao').show();
            } else {
                $('.tr_sugestao_ampliacao').hide();
            }
        });

        $('#btnSalvar').click(function() { //ppuid
            necessitasugestaoampliacao = $('[name=necessitasugestaoampliacao]:checked').val();
            
            if( (parseInt($('[name=muncod]').val()) > 0) ){
                salvarSugestaoMetaMunicipio();
            }else{
                salvarSugestaoMetaUF();
            }

            
        });
    });
        <?php if ($disabilita == 'S') { ?>
                    Disabilitado(this);
        <?php } ?>
            
        function Disabilitado() {
            jQuery("input[name='necessitasugestaoampliacao']").each(function(i) {
                jQuery(this).attr('disabled', 'disabled');
            });
        }

    function sugestaoAmpliacaoMeta(obj) {
        if (obj.value == "sim") {
            document.getElementById('tr_metasugerida').style.display = '';
            if (document.getElementById('tr_metaajustada')){
				<?php if( $habilitaMetaAjustada == 'S' ){ ?>
                	document.getElementById('tr_metaajustada').style.display = '';
                <?php }?>
            }
        }
        if (obj.value == "nao") {
            document.getElementById('tr_metasugerida').style.display = 'none';
            if (document.getElementById('tr_metaajustada'))
                document.getElementById('tr_metaajustada').style.display = 'none';
        }
    }
</script>

<?php
    #pega os tipos de meta cadastrados
	$sql = "select tpmdesc, m.tpmid, metvalor, apctermoaceito, apctermoajustadoaceito, apcampliameta from projovemcampo.meta as m
			inner join projovemcampo.tipometa as tipo on tipo.tpmid = m.tpmid
			inner join projovemcampo.adesaoprojovemcampo as ad on ad.apcid = m.apcid
			where ad.apcid = '{$_SESSION['projovemcampo']['apcid']}'";
    
	$rsDadosMeta = $db->carregar($sql);

	$sql = "select apcid, apctermoaceito, apctermoajustadoaceito, apcampliameta from projovemcampo.adesaoprojovemcampo as ad where apcid = ".$_SESSION['projovemcampo']['apcid'];
	$rsVerificaTipoMeta = $db->pegaLinha($sql);
	
	#zerando valores para metas
	unset($meta_original);
	unset($meta_sugerida);
	unset($meta_ajustada);
	
	if( $rsDadosMeta ){

		foreach( $rsDadosMeta as $meta ){
			
			if( $meta['tpmid'] == 1 ){
				
				$meta_original = $meta['metvalor'];	

			}elseif ($meta['tpmid'] == 2) {

				$meta_sugerida = $meta['metvalor'];

			}elseif ($meta['tpmid'] == 3) {

				$meta_ajustada = $meta['metvalor'];
			}
		}
	}
	
// 	ver( $meta_original, $meta_sugerida, $meta_ajustada );

    if( $meta_sugerida ){

		$total_meta = $meta_sugerida;
	
	}else{

		$total_meta = $meta_original;
	}	
?>

<form id="form" name="form" method="POST">
	<input type="hidden" name="requisicao" value="salvarTermo">  
	<input type="hidden" name="estuf" value="<?= $_SESSION['projovemcampo']['estuf']; ?>"> 
	<input type="hidden" name="muncod" value="<?= $_SESSION['projovemcampo']['muncod']; ?>"> 
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
		align="center">
		<tr>
			<td class="SubTituloDireita">Orientações:</td>
			<td>A sugestão de meta é importante para a análise pela SECADI,
				possibilitando ou não a ampliação da meta original.</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Sugestão de ampliação de meta:</td>
			<td><input type="radio" id="necessitasugestaoampliacao" name="necessitasugestaoampliacao" value="sim"
				<?=$habilita != "S" ? 'disabled="disabled"' : ''; ?>
				onclick="sugestaoAmpliacaoMeta(this);"
				<?= ($rsVerificaTipoMeta['apcampliameta'] == "t" ? "checked" : ""); ?>> Sim <input
				type="radio" id="necessitasugestaoampliacao" name="necessitasugestaoampliacao" value="nao"
				<?=$habilita != "S" ? 'disabled="disabled"' : ''; ?>
				onclick="sugestaoAmpliacaoMeta(this);"
				<?= ($rsVerificaTipoMeta['apcampliameta'] != "t") ? "checked" : ""; ?>> Não</td>
		</tr>
		<tr class="tr_sugestao_ampliacao"
			<?= ( ( $habilitaMetaSugerida == "S" && $rsVerificaTipoMeta['apcampliameta'] == "t" ) ? "" : "style=\"display:none\"") ?>
			id="tr_metasugerida">
			<td class="SubTituloDireita">Meta sugerida:</td>
			<td>
            	<?php echo campo_texto('metasugerida', 'S', $habilita, 'Meta sugerida:', 10, 8, "########", '', '', '', 0, 'id="metasugerida"', '', $total_meta); ?>
                    Estudantes
			</td>
		</tr>
		<tr
			<?= (($habilitaMetaAjustada == "S" && $rsVerificaTipoMeta['apcampliameta'] == "t" ) ? "" : "style=\"display:none\"") ?>
			id="tr_metaajustada">
			<td class="SubTituloDireita">Meta ajustada:</td>
			<td>
            	<?php echo campo_texto('metaajustada', 'S', $habilita, 'Meta ajustada', 10, 8, "########", "", '', '', 0, 'id="metaajustada"', '', $meta_ajustada); ?> 
                Estudantes
            </td>
		</tr>
		<?if($habilita == 'S'){?>
		<tr>
			<td class="SubTituloCentro" colspan="2"><input type="submit"
				class="salvar" value="Salvar"></td>
		</tr>
		<?}?>
	</table>
</form>

<?php if ($necessitasugestaoampliacao == "t"): ?>
<script>
        $('.tr_sugestao_ampliacao').show();
    </script>
<?php endif; 
registarUltimoAcesso();