<?
$perfis = pegaPerfilGeral();
$habilita = 'naopode';
$habilita1 = 'naopode';
$habilita2 = 'naopode';
$habilitapoup = 'N';
if(in_array(PERFIL_ADMINISTRADOR,$perfis)||$db->testa_superuser()){
	$habilita = 'mdaqtdcuraprov';
	$habilita1 = 'mdaqtdtecnicosadm';
	$habilita2 = 'mdaqtddocentes';
	$habilitapoup = 'N';
};
function salvar(){

	global $db;
	
	if( is_array($_POST['mdaqtdcuraprov']) ){
		$sql = '';
		foreach( $_POST['mdaqtdcuraprov'] as $mdaid => $valor ){
		
			$_POST['mdaqtdcuraprov'][$mdaid] 	= $_POST['mdaqtdcuraprov'][$mdaid] ? $_POST['mdaqtdcuraprov'][$mdaid] : 'NULL';
			$_POST['mdaqtdtecnicosadm'][$mdaid] = $_POST['mdaqtdtecnicosadm'][$mdaid] ? $_POST['mdaqtdtecnicosadm'][$mdaid] : 'NULL';
			$_POST['mdaqtddocentes'][$mdaid] 	= $_POST['mdaqtddocentes'][$mdaid] ? $_POST['mdaqtddocentes'][$mdaid] : 'NULL';
			
			$sql .= "UPDATE 
						academico.medidascursos 
					SET
						mdaqtdcuraprov = {$_POST['mdaqtdcuraprov'][$mdaid]},
						mdaqtdtecnicosadm = {$_POST['mdaqtdtecnicosadm'][$mdaid]},
						mdaqtddocentes = {$_POST['mdaqtddocentes'][$mdaid]}
					WHERE
						mdaid = $mdaid; ";
		}
		if( $sql != '' ){
			$db->executar($sql);
			$db->commit();
		}
	}
	echo "
		<script>
			alert('Dados Salvos com sucesso!');
			window.location = window.location;
		</script>";
}
        
if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

monta_titulo('Câmpus com Menos de 5 Cursos', '');
?>	
<style>
.imgbot{
	cursor:pointer;
}
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
$(document).ready(function(){
	$('.alterar').click(function(){ 
		var mdaid = $(this).attr('id');
		if( $(this).attr('src') == '../imagens/alterar_pb.gif' ){
			$(this).attr('src','../imagens/alterar.gif');
			$(this).attr('title','Editar');
			$('[name*="<?=$habilita ?>['+mdaid+']"]').attr('disabled',true);
			$('[name*="<?=$habilita ?>['+mdaid+']"]').attr('readonly',true);
			$('[name*="<?=$habilita ?>['+mdaid+']"]').removeClass('normal');
			$('[name*="<?=$habilita ?>['+mdaid+']"]').addClass('disabled');
			$('[name*="<?=$habilita1 ?>['+mdaid+']"]').attr('disabled',true);
			$('[name*="<?=$habilita1 ?>['+mdaid+']"]').attr('readonly',true);
			$('[name*="<?=$habilita1 ?>['+mdaid+']"]').removeClass('normal');
			$('[name*="<?=$habilita1 ?>['+mdaid+']"]').addClass('disabled');
			$('[name*="<?=$habilita2 ?>['+mdaid+']"]').attr('disabled',true);
			$('[name*="<?=$habilita2 ?>['+mdaid+']"]').attr('readonly',true);
			$('[name*="<?=$habilita2 ?>['+mdaid+']"]').removeClass('normal');
			$('[name*="<?=$habilita2 ?>['+mdaid+']"]').addClass('disabled');
		}else{
			$(this).attr('src','../imagens/alterar_pb.gif');
			$(this).attr('title','Bloquear');
			$('[name*="<?=$habilita ?>['+mdaid+']"]').attr('disabled',false);
			$('[name*="<?=$habilita ?>['+mdaid+']"]').attr('readonly',false);
			$('[name*="<?=$habilita ?>['+mdaid+']"]').removeClass('disabled');
			$('[name*="<?=$habilita ?>['+mdaid+']"]').addClass('normal');
			$('[name*="<?=$habilita1 ?>['+mdaid+']"]').attr('disabled',false);
			$('[name*="<?=$habilita1 ?>['+mdaid+']"]').attr('readonly',false);
			$('[name*="<?=$habilita1 ?>['+mdaid+']"]').removeClass('disabled');
			$('[name*="<?=$habilita1 ?>['+mdaid+']"]').addClass('normal');
			$('[name*="<?=$habilita2 ?>['+mdaid+']"]').attr('disabled',false);
			$('[name*="<?=$habilita2 ?>['+mdaid+']"]').attr('readonly',false);
			$('[name*="<?=$habilita2 ?>['+mdaid+']"]').removeClass('disabled');
			$('[name*="<?=$habilita2 ?>['+mdaid+']"]').addClass('normal');
		}
	});
	$('.inserir').click(function(){
		var janela = window.open( 'academico.php?modulo=principal/mpa/popupMpaPlanoExpEnsMed&acao=A', 'adiciona_campus', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
	});
    $('.documentos').click(function(){
		var id = $(this).attr('id');
		var janela = window.open( 'academico.php?modulo=principal/monitoramentoProgramasAcoes&acao=A&popup=popupMostraDocumentos&mdaid='+id, 'altera_documento', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
    });
    $('.demanda').click(function(){
		var id = $(this).attr('id');
		var janela = window.open( 'academico.php?modulo=principal/monitoramentoProgramasAcoes&acao=A&popup=popupMostraDemanda&mdaid='+id+'&habilita=<?=$habilitapoup?>', 'altera_demanda', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
    });
    $('.cursos').click(function(){
		var id = $(this).attr('id');
		var janela = window.open( 'academico.php?modulo=principal/monitoramentoProgramasAcoes&acao=A&popup=popupMostraCursos&cmpid='+id, 'altera_cursos', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
    });
	
    $('.salvar').click(function(){
        $('#req').val('salvar');
        $('#formulario').submit();
    });
});
</script>
<form id="formulario" name="formulario" method="post" action="">
	<input type="hidden" value="" name="req" id="req" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
 			<td>
	 			<table cellspacing="0" cellpadding="2" border="0" align="center" width="100%" class="listagem" style="color:333333;">
 			<? 
			switch ( $_SESSION['academico']['orgid'] ){
				case '1':
					$funid = ACA_ID_CAMPUS;
				break;
				case '2':
					$funid = ACA_ID_UNED;
				break;
			}
 			
 			$sql = "SELECT DISTINCT
						e2.entnome,
						mda.mdaid,
						cmp.cmpid,
						ccu.qtd,
						count(DISTINCT amdid) as qtd_arq,
						mdaqtdcuraprov,
						mdaqtdtecnicosadm,
						mdaqtddocentes
					FROM
						academico.campus cmp
					INNER JOIN entidade.entidade e2 ON e2.entid = cmp.entid
					INNER JOIN entidade.entidade e ON e2.entid = e.entid
					INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
					INNER JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid 
					LEFT  JOIN academico.medidascursos mda ON mda.cmpid = cmp.cmpid
					LEFT  JOIN academico.arqmedidas amd ON amd.mdaid = mda.mdaid 
					LEFT  JOIN (
						SELECT
							cmpid,
							count(DISTINCT curid) as qtd
						FROM
							academico.campuscurso
						WHERE
							cpcano = '2013' 
							AND cpcprevisto = 'T'
						GROUP BY
							cmpid) ccu ON ccu.cmpid = cmp.cmpid  
					WHERE
						ea.entid = '{$_SESSION['academico']['entid']}'
						AND qtd < 5
						AND ef.funid = {$funid}
					GROUP BY
						e2.entnome,
						mda.mdaid,
						cmp.cmpid,
						ccu.qtd,
						mdaqtdcuraprov,
						mdaqtdtecnicosadm,
						mdaqtddocentes";
 			
 			$campus = $db->carregar($sql);
 			$cabecalho = Array('Ação','Campus','N° de Cursos','MEC-Cursos Aprovados','MEC-Técnicos Adminsitrativos','MEC-Docentes','Medidas a Serem Adotadas','Documentos Anexos');
 			if( is_array($campus) ){
 			?>
	 				<tr>
	 					<? foreach( $cabecalho as $cab ){ ?>
	 					<td align="center" valign="top" 
	 						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
	 						class="title">
	 						<?=$cab ?>
	 					</td>
	 					<? } ?>
	 				</tr>
 			<? 
 				foreach( $campus as $camp ){
 					if( $camp['mdaid'] == '' ){
 						$sql = "INSERT INTO academico.medidascursos( cmpid)
							    VALUES ({$camp['cmpid']})
							    RETURNING mdaid;";
 						$camp['mdaid'] = $db->pegaUm($sql);
 						$db->commit();
 					}
 			?>
	 				<tr bgcolor="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#ffffcc';">
	 					<td valign="top" title="Ação" align="center">
	 						<img border="0" src="../imagens/alterar.gif" title="Alterar" class="alterar" id="<?=$camp['mdaid'] ?>">
	 					</td>
	 					<td valign="top" title="Campus"><?=$camp['entnome'] ?></td>
	 					<td valign="top" title="Ver Cursos" align="center" <?=($camp['qtd'] == '5' ? 'style="color:red"' : ''); ?> >
	 						<?=$camp['qtd'] ?> 
	 						<img border="0" style="cursor:pointer" id="<?=$camp['cmpid'] ?>" class="cursos" src="../imagens/consultar.gif">
	 					</td>
	 					<td valign="top" title="Ação" align="center">
	 						<?=campo_texto("mdaqtdcuraprov[{$camp['mdaid']}]",'N',"N",'',3,2,'##','', '', '', '', 'id="mdaqtdcuraprov'.$camp['mdaid'].'"','',$camp['mdaqtdcuraprov']);?>
	 					</td>
	 					<td valign="top" title="Ação" align="center">
	 						<?=campo_texto("mdaqtdtecnicosadm[{$camp['mdaid']}]",'N',"N",'',3,2,'##','', '', '', '', 'id="mdaqtdtecnicosadm'.$camp['mdaid'].'"','',$camp['mdaqtdtecnicosadm']);?>
	 					</td>
	 					<td valign="top" title="Ação" align="center">
	 						<?=campo_texto("mdaqtddocentes[{$camp['mdaid']}]",'N',"N",'',3,2,'##','', '', '', '', 'id="mdaqtddocentes'.$camp['mdaid'].'"','',$camp['mdaqtddocentes']);?></td>
	 					<td valign="top" title="Inserir Demanda" align="center">
		 					<img border="0" id="<?=$camp['mdaid'] ?>"  style="cursor:pointer"
		 						 class="demanda" src="../imagens/consultar.gif"></td>
	 					<td valign="top" title="Inserir Documento" align="center">
	 						<img border="0" style="cursor:pointer" id="<?=$camp['mdaid'] ?>"class="documentos" 
	 							 src="../imagens/alterar.gif">     
	 						<?=$camp['qtd_arq'] ?>
	 					</td>
					</tr>
 			<?
 				}
 			}
 			?>
	 			</table>
	        </td>
	    </tr>
		<tr>
 			<td>
	        	<input type="button" class="salvar" style="cursor:pointer" value="Salvar">
	        </td>
	    </tr>    
	</table>
</form>
