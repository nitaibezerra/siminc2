<?php 

if($_REQUEST['anexo']=='download'){
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$file->getDownloadArquivo($_REQUEST['arqid']);
	die;
}

require_once APPRAIZ . "maismedicos/classes/Pagamento_Autorizacao.class.inc";
$obAut = new Pagamento_Autorizacao();

if($_REQUEST['requisicao']){
	$obAut->$_REQUEST['requisicao']($_POST);
	die;
}

require_once APPRAIZ . "www/includes/webservice/cpf.php";
require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
$db->cria_aba( $abacod_tela, $url, $parametros);
monta_titulo('Validação de Bolsas', ($_GET['ano'] ? mes_extenso($_GET['mes']).'/'.$_GET['ano'] : ''));
?>
<!-- <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script> -->
<link href="../includes/JQuery/jquery-1.9.1/css/jquery-ui-1.10.3.custom.css" rel="stylesheet">
<script src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
<script src="../includes/JQuery/jquery-1.9.1/jquery-ui-1.10.3.custom.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1/funcoes.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script>
	$(function(){

		$("#modalDialog").dialog({
			height: $(document).height()-20,
            width: $(document).width()-20,
		      autoOpen: false,
		      show: {
		        effect: "blind",
		        duration: 1000
		      },
		      hide: {
		        effect: "explode",
		        duration: 1000
		      }
		});
		
		$('.btnAbrir').click(function(){
			mes = $('[name=apgmes]').val();
			ano = $('[name=apgano]').val();
			if(mes && ano){
				document.location.href = document.location.href+'&mes='+mes+'&ano='+ano;
				return true
			}
			alert('Selecione um mês e ano!');
		});


		$('.btnValida').click(function(){
						
			arParams = this.id.split('_');
			img = $(this).attr('src');
			cpf = arParams[0];
			mes = arParams[1];
			ano = arParams[2];

			//$( '#modalDialog' ).attr('title','Validar Bolsa do CPF '+cpf);
			$.ajax({
				url	: document.location.href,
				type	: 'post',
				data	: '&requisicao=validaBolsa&cpf='+cpf+'&mes='+mes+'&ano='+ano+'',
				success	: function(e){
					$( '#modalDialog' ).html(e);
					$( '#modalDialog' ).dialog('open');
					}
				});
			
		});

		$('.btnFecharModal').click(function(){
			$( '#modalDialog' ).dialog('close');	
		});
	});
	
</script>
<div style="padding:5px;text-align:justify;display:none;" id="modalDialog" title="Validar Bolsa">
	
</div>
<?php if(empty($_GET['mes']) && empty($_GET['ano'])): ?>
	<center>
		<h3>Selecione o mês para pagamento</h3>
		<p>
			<?php 
			$a=0;
			for($x=1;$x<13;$x++){
				$meses_periodo[$a]['codigo'] = str_pad($x,2,'0', STR_PAD_LEFT);
				$meses_periodo[$a]['descricao'] = mes_extenso($x);
				$a++;
			}
			$apgmes = date('m');
			$db->monta_combo('apgmes',$meses_periodo,'S','Mês',null,null,null,null,'N','apgmes');
			
			$b=0;
			for($y=2012;$y<date('Y')+2;$y++){
				$anos_periodo[$b]['codigo'] = $y;
				$anos_periodo[$b]['descricao'] = $y;
				$b++;
			}
			$apgano = date('Y');
			$db->monta_combo('apgano',$anos_periodo,'S','Ano',null,null,null,null,'N','apgano');
			?>&nbsp;
			<input type="button" value="Abrir" class="btnAbrir" />
		</p>
	</center>
	
<?php else: ?>

	<input type="hidden" name="apgmes" value="<?php echo $_GET['mes']?>" />
	<input type="hidden" name="apgano" value="<?php echo $_GET['ano']?>" />
	<?php
	$perfis = arrayPerfil();
	if(in_array(PERFIL_COORDENADOR_CAMEM, $perfis)){
		$arWhere[] = "fun.funid = ".FUNCAO_AVALIADOR_CAMEM;
	}else
	if(in_array(PERFIL_DIRETOR_CAMEM, $perfis)){
		$arWhere[] = "fun.funid = ".FUNCAO_COORDENADOR_CAMEM;
	}else
	if(!$db->testa_superuser()){
		$arWhere[] = "ben.benid is null";
	}
	$sql = "select distinct 
				case when ben.benstatus = 'A' then '<img src=\"../imagens/0_ativo.png\" id=\"' || ben.benid || '\" class=\"btnStatus\" />'
				else '<img src=\"../imagens/0_inativo.png\" id=\"' || ben.benid || '\" class=\"btnStatus\" />' end as status,
				ben.bencpf,
				ben.bennome,
				fun.funnome as funcao,
				uni.uninome,
				mun.mundescricao || '/' || mun.estuf as municipio,
				case when apg.apgmes is not null then apg.apgmes || '/' || apg.apgano
				else '{$_GET['mes']}/{$_GET['ano']}' end as mes_ref,
				
				case when pte.ptevalida is not null then
				'<img src=\"../imagens/aceite_os_2.png\" id=\"' || ben.bencpf || '_' || {$_GET['mes']} || '_' || {$_GET['ano']} || '\" class=\"btnValida\" style=\"cursor:pointer\"/>'
				else
				'<img src=\"../imagens/aceite_os_2_inativo.png\" id=\"' || ben.bencpf || '_' || {$_GET['mes']} || '_' || {$_GET['ano']} || '\" class=\"btnValida\" style=\"cursor:pointer\"/>' end as valida,

				(select coalesce(count(*),0) from maismedicos.planotrabalho_visitaprogramada ptv
				join maismedicos.planotrabalho ptr on ptv.ptrid = ptr.ptrid
				where ptr.ptrstatus = 'A' and ptv.ptvstatus = 'A' and ptr.ptrcpf=ben.bencpf
				--and ptr.ptrmes = '{$_REQUEST['mes']}' and ptr.ptrano = '{$_REQUEST['ano']}'
				and to_char(ptvdtprevini,'YYYYMM')>='{$_REQUEST['ano']}{$_REQUEST['mes']}' 
			 	and to_char(ptvdtprevfim,'YYYYMM')<='{$_REQUEST['ano']}{$_REQUEST['mes']}'
				) as visitas,
				(select coalesce(count(*),0) from maismedicos.planotrabalho_atividadeacompanhamento pta
				join maismedicos.planotrabalho ptr on pta.ptrid = ptr.ptrid
				where ptr.ptrstatus = 'A' and pta.ptastatus = 'A' and ptr.ptrcpf=ben.bencpf
				--and ptr.ptrmes = '{$_REQUEST['mes']}' and ptr.ptrano = '{$_REQUEST['ano']}'
				and to_char(ptadtprevisto,'YYYYMM')='{$_REQUEST['ano']}{$_REQUEST['mes']}'
				) as atividades,
				
				case when apg.rmdid is null then 'Não'
				else 'Sim' end as remessa
			from maismedicos.pagamento_beneficiario ben	
			left join maismedicos.pagamento_beneficiariofuncao fun on fun.funid = ben.funid		
			left join maismedicos.universidade uni on uni.uniid = ben.uniid
			left join territorios.municipio mun on mun.muncod = ben.muncod
			left join maismedicos.pagamento_autorizacao apg on ben.bencpf = apg.apgcpf
				and apg.apgmes = '{$_REQUEST['mes']}' and apg.apgano = '{$_REQUEST['ano']}'
				and apg.apgstatus = 'A'
			left join maismedicos.planotrabalho_execucao pte on pte.ptecpf = ben.bencpf
				and pte.ptestatus = 'A'
				and pte.ptemes = '{$_REQUEST['mes']}' and pte.pteano = '{$_REQUEST['ano']}'
			where ben.benstatus = 'A'
				".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '');
	
	$arCabecalho = array('Status', 'CPF', 'Nome', 'Função', 'Instituição', 'Município', 'Mês', 'Validar Bolsa', 'Visitas','Atividades','Gerado Remessa?');
	$db->monta_lista($sql, $arCabecalho, 100000, 10, '', '', '', '', '');
	?>
	
	<center>
	<p>&nbsp;</p>
	<p>
		<input type='button' value="Voltar" onclick="document.location.href='maismedicos.php?modulo=principal/pagamento/validaAutorizacaoPagamento&acao=A'" />
	</p>
	</center>
	
<?php endif; ?>