<div style="width: 70%;margin:0 auto; padding: 10px 0 0 0;">
    <h1 id="h1contribuicao" style="display:none; margin:15px auto 0 auto; text-align: center;">Metas PNE - <?= $_SESSION['par']['mundescricao'] ?></h1>
</div>
<form name='formulario' action='' method='post'>
	<input type='hidden' name='metid' value = ''  id = "metid" >
    <input type='hidden' name='requisicao' value = ''  id = "requisicao" >
    <input type='hidden' name='tipo' value = ''  id = "tipo" >
    <table style="width: 100%!important;">
    	<tr>
        	<td colspan="2">
            	<?php
                	echo montarAbasArrayLocal( criarAbasMetasPNE() , "" );
				?>
			</td>
		</tr>
        <tr>
        	<td  id="esconder"  style="display:none;"class="" width="20%" valign="top">
            	<div style="margin-top:20px;" id="pesquisa">
                	<div style="float:left;" class="titulo_box" >
                    	Pesquisa<br/><br/>
					</div>
				</div>
				<table  cellpadding="5" cellspacing="1" width="100%" id="tabelaRegioes">
                	<?php
                    	$sql = " Select	regcod AS codigo, regdescricao AS descricao From territorios.regiao order by regdescricao";
                        mostrarComboPopupLocal( 'Região', 'slRegiao',  $sql, "", 'Selecione as Regiões', null,'atualizarRelacionadosRegiao(1)',false);
					?>
				</table>
                <table  cellpadding="5" cellspacing="1" width="100%" id = "tabelaEstados" class="filtro_combo">
                	<?php
                    	listarEstados();
					?>
				</table>
                <table  cellpadding="5" cellspacing="1" width="100%" id = "tabelaMesoregioes" class="filtro_combo">
                	<?php
                    	listarMesoregioes();
					?>
				</table>
			</td>
			<td class="" width="80%" id="divListagem" valign="top"></td>
		</tr>
	</table>
</form>