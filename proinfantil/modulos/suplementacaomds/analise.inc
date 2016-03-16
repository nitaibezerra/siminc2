<?php 

if( $_SESSION['proinfantil']['mds']['muncod'] < 1 ){
	echo "
		<script>
			alert('Problemas de sessão.');
			window.location.href = 'proinfantil.php?modulo=inicio&acao=C';
		</script>";
	die();
}

if($_POST['historico']){
	header('content-type: text/html; charset=ISO-8859-1');
	$sql_historico = "SELECT 		u.usunome, p.mdaanalisetecnica, t.tipdescricao, to_char(p.mdadata,'DD/MM/YYYY'),CASE WHEN p.mdastatus = 'A' THEN 'Ativo' ELSE 'Inativo' END mdastatusmdastatus
					  FROM 			proinfantil.mdsanalise p
					  LEFT JOIN 	seguranca.usuario u ON u.usucpf = p.usucpf
					  LEFT JOIN 	proinfantil.tipoparecer t ON t.tipid = p.tipid
					  WHERE 		p.muncod = '{$_SESSION['proinfantil']['mds']['muncod']}' and p.mdaano = '{$_SESSION['exercicio']}'
					  ORDER BY 		mdaid DESC";
	
	$historico = $db->carregar($sql_historico);
	$cab = array('Analista','Parecer','Tipo de parecer','Data','Status');
	$db->monta_lista($sql_historico, $cab, 1000, 10, 'N', '', '', '', '', ''); 
	exit();
}

if($_REQUEST['requisicao'] == 'salvarAnalise'){
	$mdaid = $_REQUEST['mdaid'];
	$muncod = $_SESSION['proinfantil']['mds']['muncod'];
	$mdavalorderepasse = $_REQUEST['mdavalorderepasse'] ? $_REQUEST['mdavalorderepasse'] : '0';
	$mdaanalisetecnica = $_REQUEST['mdaanalisetecnica'];
	
	$arMdsconferido = !empty($_REQUEST['mdsconferido']) ? explode(',', $_REQUEST['mdsconferido']) : array();
	
	$sql = '';
	foreach($arMdsconferido as $mdsconferido){
		if($mdsconferido){		
			$arMdsconferido = explode('_', $mdsconferido);			
			$sql .= "UPDATE 	proinfantil.mdssuplementacao  
					 SET		mdsconferido = '{$arMdsconferido[0]}'
					 WHERE 		mdsid = {$arMdsconferido[1]};";			
			unset($arMdsconferido);
		}
	}
		
	if($sql){
		$db->executar($sql);
	}

	if($db->commit()){
		$db->sucesso('suplementacaomds/analise');
	}	
}

if($_REQUEST['requisicao'] == 'salvarParecer'){
	
	$mdaid = $_REQUEST['mdaid'];
	$muncod = $_SESSION['proinfantil']['mds']['muncod'];
	$mdaanalisetecnica = $_REQUEST['mdaanalisetecnica'];
	$mdavalorderepasse = '0';
	$tipid = $_REQUEST['tipid'];
	$usuario = $_SESSION['usucpf'];
	$data = date('Y-m-d');
	$sql = "";
	$sql = "SELECT 	MAX(mdaid) as mdaid
			FROM  	proinfantil.mdsanalise 
			WHERE 	muncod = '{$muncod}' AND mdastatus = 'A' and mdaano = '{$_SESSION['exercicio']}'";
	
	$res = $db->pegaLinha($sql);

	if(empty($res['mdaid'])){
		$sql_ins = "INSERT INTO proinfantil.mdsanalise
				(muncod, mdavalorderepasse, mdaanalisetecnica, mdadata, mdastatus, usucpf, tipid, mdaano)
				VALUES
				('{$muncod}','{$mdavalorderepasse}','{$mdaanalisetecnica}','{$data}','A','{$usuario}',$tipid, '{$_SESSION['exercicio']}')";
		$db->executar($sql_ins);
		if($db->commit()){
			$db->sucesso('suplementacaomds/analise');
		}			
	} else {
		$sql_up = "UPDATE 	proinfantil.mdsanalise 
				   SET	    mdastatus = 'I' 
				   WHERE 	muncod = '{$muncod}'
				   AND 		mdaid = {$mdaid}
				   and 		mdaano = '{$_SESSION['exercicio']}'";
		$db->executar($sql_up);
		
		$sql_in = "INSERT INTO proinfantil.mdsanalise
				(muncod, mdavalorderepasse, mdaanalisetecnica, mdadata, mdastatus, usucpf, tipid, mdaano)
				VALUES
				('{$muncod}','{$mdavalorderepasse}','{$mdaanalisetecnica}','{$data}','A','{$usuario}',$tipid, '{$_SESSION['exercicio']}')";
		$db->executar($sql_in);
		
		if($db->commit()){
			$db->sucesso('suplementacaomds/analise');
		}	
	} 
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
include_once APPRAIZ . "includes/workflow.php";
include  "_funcoes_mds.php";
echo "<br/>";
?>

<style>
.div_historico {
background: #white;
}
.div_historico:hover {
background: #eeeeaa;
}
</style>

<table class="tabela" height="100%" align="center" cellspacing="0" cellpadding="3" >
	<tr class="subtituloEsquerda">
		<td height="10" width="45%">
			<input type="button" value="Voltar" onclick="javascript:document.location.href='proinfantil.php?modulo=suplementacaomds/listaCreche&acao=A'" id="btnVoltar" />
		</td>
		<td class="TituloTela">Suplementação MDS</td>
	</tr>
</table>
<br/>
<?php 
$abacod_tela = 57568;

$arMnuid = array();

echo $db->cria_aba( $abacod_tela, $url, '', $arMnuid );

$linha1 = 'Análise'; 
$linha2 = '';
monta_titulo($linha1, $linha2);

cabecalhoMunicipio();

$sql = "SELECT	cpmid 
		FROM 	proinfantil.mdsdadoscriancapormunicipio cpm		 
		WHERE	muncod = '{$_SESSION['proinfantil']['mds']['muncod']}' and cpm.cpmano = ({$_SESSION['exercicio']} - 1)";

$cpmid = $db->pegaUm($sql);

$docid = criaDocumento($cpmid);
$esdid = pegaEstadoAtual($docid);

$acesso = verificaAcessoPerfil($_SESSION['usucpf'],$esdid);

$sql = "SELECT 		mdaid,
					muncod,
					mdavalorderepasse,
					mdaanalisetecnica,
					tipid
		FROM		proinfantil.mdsanalise
		WHERE		muncod = '{$_SESSION['proinfantil']['mds']['muncod']}' AND mdastatus = 'A' and mdaano = '{$_SESSION['exercicio']}'
		ORDER BY 	mdaid desc";

$rs = $db->pegaLinha($sql);

if($rs) extract($rs);
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
	$(function(){
		function aguarda(val){
			if(val){
				$('#aguardando').show();
			}else{
				$('#aguardando').hide();
			}
		}
		$('.div_historico').click(function(){
			aguarda(true);
			var id = $(this).attr('id');
			if($('#td_'+id).html() != ''){
				if( $('#td_'+id).css('display') == 'none' ){
					$('#td_'+id).show();
					$('#imgmais'+id).hide();
					$('#imgmenos'+id).show();
				} else {
					$('#td_'+id).hide();
					$('#imgmais'+id).show();
					$('#imgmenos'+id).hide();
				}
				aguarda(false);
			} else {
				$.ajax({
					type: "POST",
					url: 'proinfantil.php?modulo=suplementacaomds/analise&acao=A',
					data: "historico=true&id="+id,
					async: false,
					success: function(msg){
						$('#td_'+id).html(msg);
						$('#td_'+id).show();
						$('#imgmais'+id).hide();
						$('#imgmenos'+id).show();					
						aguarda(false);
					}
				});
			}
		});
		aguarda(false);
	});

	$(function(){
		$('.btnSalvarParecer').click(function(){
			if($('[name=mdaanalisetecnica]').val() == ''){
				alert('O campo descrição da análise é obrigatório!');
				$('[name=mdaanalisetecnica]').focus();
				return false;
			}	
			$('[name=tipid]').attr('id','tipid');
			if($('#tipid:checked').length == 0){
				alert('Selecione o Tipo de parecer!');
				$('[name=tipid]').focus();
				return false;
			} 
			$('#formulario_parecer').submit();
		});
	});

	$(function(){
		$('.btnSalvar').click(function(){
//			if($('[name=mdavalorderepasse]').val() == ''){
//				alert('O campo valor do repasse está com valor zerado!');
//				$('[name=mdavalorderepasse]').focus();
//				return false;
//			}

			var arMdsconferido = new Array;
			$.each($('[name^="mdsconferido_"]:checked'), function(i, v){
				if(v.value)
					arMdsconferido.push(v.value);				
			});
			
			$('[name=mdsconferido]').val(arMdsconferido);
			$('#formulario').submit();
		});
	});
	
	function popGuiaParecer(modid,campoTipo,campoParecer){
		var cod = 0;
		var campo = document.getElementsByName("tipid");
		
		for(i=0; i< campo.length; i++){
			if(campo[i].checked == true) cod = campo[i].value;
		}
		
		if(cod == 0){
			alert("Selecione o Tipo de parecer!");
			return false;
		}
		
		w = window.open('?modulo=principal/popGuiaParecer&acao=A&modid='+modid+'&filtraTipo='+cod+'&campoTipo='+campoTipo+'&campoParecer='+campoParecer,'guiaparecer','scrollbars=yes,location=no,toolbar=no,menubar=no,width=550,height=200,left=250,top=125'); 
		w.focus();	
	}
	function carregarParcerAnalise( parecer, tipid, campoTipo, campoParecer ){
		jQuery('[name="'+campoParecer+'"]').val(parecer);
		jQuery('[name="'+campoTipo+'"]').val(tipid);	
	}
</script>
<form name="formulario" id="formulario_parecer" method="post" action="">
	<input type="hidden" name="requisicao" value="salvarParecer" />
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="2">		
		<!--  
		<tr>
			<td class="subtituloDireita" width="120">Valor de Repasse para o Município</td>
			<td>
				Cálculo do campo acima: (Total de creche Integral informada da Educação Infantil * 2.725,69) + (Total de creche parcial informada da Educação Infantil  * 1.677,35) +  (Total de creche Integral informada da Instituições conveniadas * 2.306,35) + (Total de creche parcial informada das Instituições conveniadas * 1.677,35)
				Privada/Conveniada: (Qtd Parcial) * (Valor de Repasse Parcial) + (Qtd Integral) * (Valor de Repasse Integral) <br/>
				Municipal: (Qtd Parcial) * (Valor de Repasse Parcial) + (Qtd Integral) * (Valor de Repasse Integral) <br/>
				<?php 
				/*
				$sql = "SELECT sum(total.valorderepasse) as total
						FROM (
						-- ESCOLAS PÚBLICAS -- Valores dividio por 1/4 (Ver no próximo ano a regra será valores 1/2)
						            SELECT           (sum(coalesce(sup.mdsquantidadepbfparcial,0) * 419.34) + sum(coalesce(sup.mdsquantidadepbfintegral,0) * 681.42))::numeric as valorderepasse
						            FROM entidade.entidade ent
						            INNER JOIN entidade.endereco ede on ede.entid = ent.entid
						            INNER JOIN territorios.municipio mun on mun.muncod = ede.muncod
						            INNER JOIN proinfantil.procenso pro on pro.entcodent = ent.entcodent
						            INNER JOIN proinfantil.mdssuplementacao sup on sup.entcodent = ent.entcodent
						            WHERE mun.muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'
						            AND ent.tpcid in (1,2,3)
						                        
						UNION ALL
						
						-- ESCOLAS Privadas  -- Valores dividio por 1/4 (Ver no próximo ano a regra será valores 1/2)
						            SELECT           (sum(coalesce(sup.mdsquantidadepbfparcial,0) * 419.34) + sum(coalesce(sup.mdsquantidadepbfintegral,0) * 576.59))::numeric as valorderepasse
						
						            FROM entidade.entidade ent
						
						            INNER JOIN entidade.endereco ede on ede.entid = ent.entid
						
						            INNER JOIN territorios.municipio mun on mun.muncod = ede.muncod
						
						            INNER JOIN proinfantil.procenso pro on pro.entcodent = ent.entcodent
						
						            INNER JOIN proinfantil.mdssuplementacao sup on sup.entcodent = ent.entcodent
						
						            WHERE           mun.muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'
						
						                        AND ent.tpcid in (4)
						
						) AS total";
				
				$total = $db->pegaUm($sql);
				
				if($total){
					echo formata_valor($total);
				}else{
					echo '0,00';
				}
				*/
				?>				
				<input type="hidden" name="mdavalorderepasse" value="<?php //echo $total; ?>" />				
			</td>
		</tr>	
		-->
		<tr>
			<td class="subtituloDireita" width="120">Tipo de parecer</td>
			<td>
			<?php 
				$sql_tparecer = "SELECT	 tipid as codigo, tipdescricao as descricao
								 FROM 	 proinfantil.tipoparecer";
				$db->monta_radio('tipid',$sql_tparecer,'S',0);	?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="120">Análise Técnica</td>
			<td>
				<input type="hidden" name="mdaid" value="<?php echo $mdaid ? $mdaid : '' ?>" />	
				
				<?php if($acesso['analise']=='S'){?>	
				<div style="position:absolute; padding-left: 34%">
					<img src="../imagens/consultar.gif" style="cursor:pointer;vertical-align: top" onclick="popGuiaParecer('3','tipidParecer','mdaanalisetecnica')">
				</div>
				<?php } ?>
				
				<?php echo campo_textarea('mdaanalisetecnica', 'S', $acesso['analise'], '', 95, 15, 4000, ''); ?>
				
				<input type="hidden" name="tipidParecer" id="tipidParecer" value="">
				
			</td>
		</tr>
		<?php if($acesso['analise'] == 'S' || $_SESSION['data_hora_atual'] >= $_SESSION['data_hora_final']): ?>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input id="btnSalvarParecer" type="button" value="Salvar Parecer" class="btnSalvarParecer" />
			</td>
		</tr>
		<?php endif; ?>
	</table>
</form>
<br>

<center>
	<div id="aguardando" style="display:none; position: absolute; background-color: white; height:100%; width:100%; opacity:0.4; filter:alpha(opacity=40)" >
		<div style="margin-top:250px; align:center;">
			<img border="0" title="Aguardando" src="../imagens/carregando.gif">
			Carregando...
		</div>
	</div>
</center>

<?php 
$sql_historico = "SELECT 		u.usunome, p.mdaanalisetecnica, t.tipdescricao, to_char(p.mdadata,'DD/MM/YYYY'),CASE WHEN p.mdastatus = 'A' THEN 'Ativo' ELSE 'Inativo' END mdastatusmdastatus
				  FROM 			proinfantil.mdsanalise p
				  LEFT JOIN	 	seguranca.usuario u ON u.usucpf = p.usucpf
				  LEFT JOIN 	proinfantil.tipoparecer t ON t.tipid = p.tipid
				  WHERE 		p.muncod = '{$_SESSION['proinfantil']['mds']['muncod']}' and p.mdaano = '{$_SESSION['exercicio']}'
				  ORDER BY 		mdaid DESC";
$historico = $db->pegaLinha($sql_historico);
if(!empty($historico)){?>
<table border="0" class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" >
	<tr>
		<td style="background-color: #CDCDCD;">
			<div id="hist" class="div_historico" style="float:left;cursor:pointer;width:100%">
				<img src="../imagens/mais.gif" style="cursor:pointer; padding-left: 5px; padding-top: 5px;" id="imgmaishist">
				<img src="../imagens/menos.gif" style="cursor:pointer; display: none; padding-left: 5px; padding-top: 5px;" id="imgmenoshist">
				<center><b>Histórico de Pareceres</b></center>
			</div>
		</td>
	</tr>
	<tr>
		<td>
			<div id="td_hist" style="display:none;"></div>
		</td>
	</tr>	
</table>

<br>
<?php } ?>

<form name="formulario" id="formulario" method="post" action="">
	<input id="requisicao" type="hidden" name="requisicao" value="salvarAnalise" />
	<input id="mdsconferido" type="hidden" name="mdsconferido" value="" />	
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
		<tr>
			<td class="subtituloCentro" style="background-color: #CDCDCD;">Creches Municipais</td>
		</tr>
		<tr>		
			<td valign="top">
				<?php 
				
				if($entnome){
					$arWhere[] = " ent.entnome ilike '%{$entnome}%' ";	
				}
				
				if($entcodent){
					$arWhere[] = " ent.entcodent ilike '%{$entcodent}%' ";
				}
				
				$stCampoParcialCenso = "";
				$stCampoIntegralCenso = "";
				$cabecalho = array('Cod. INEP', 'Nome da Escola', 'Qtd. de crianças na Creche Integral', 'Qtd. Crianças de 0 a 48 meses - Bolsa Família - Integral', 'Qtd. de crianças na Creche Parcial', 'Qtd. Crianças de 0 a 48 meses - Bolsa Família - Parcial', 'Analisado');
				
				$perfis = pegaPerfil($_SESSION['usucpf']);
				
				if( !in_array(EQUIPE_MUNICIPAL,$perfis) || !in_array(SECRETARIO_ESTADUAL,$perfis)){
					$stCampoParcialCenso = "coalesce(prcnummatcreche0a48parcial, 0) as prcnummatcreche0a48parcial,";
					$stCampoIntegralCenso = "coalesce(prcnummatcreche0a48integral, 0) as prcnummatcreche0a48integral,";				
					$cabecalho = array('Cod. INEP', 'Nome da Escola', 'Qtd. de crianças na Creche Integral', 'Qtd. Crianças de 0 a 48 meses - Bolsa Família - Integral', 'Matrículas de 0 a 48 meses Integral - Censo', 'Qtd. de crianças na Creche Parcial', 'Qtd. Crianças de 0 a 48 meses - Bolsa Família - Parcial', 'Matrículas de 0 a 48 meses Parcial - Censo', 'Analisado');
				}
				
				$sql = "SELECT 		DISTINCT ent.entcodent||'&nbsp;' as entcodent,
						            ent.entnome,
						            coalesce(pro.prcqtdalunoinfantilintegral,0) as prcqtdalunoinfantilintegral,
						            coalesce(mdsquantidadepbfintegral,0) as campo_integral,
						            {$stCampoIntegralCenso}
						            coalesce(pro.prcqtdalunoinfantilparcial,0) as prcqtdalunoinfantilparcial,			            
						            coalesce(mdsquantidadepbfparcial,0) as campo_parcial,			            
						            {$stCampoParcialCenso}
						            '			            
						            <input type=\"radio\" name=\"mdsconferido_' || sup.mdsid || '\" value=\"t_' || sup.mdsid || '\" ' || case when mdsconferido = 't' then 'checked' else ' ' end || '/>&nbsp;Sim
						            <input type=\"radio\" name=\"mdsconferido_' || sup.mdsid || '\" value=\"f_' || sup.mdsid || '\" ' || case when mdsconferido = 'f' then 'checked' else ' ' end || '/>&nbsp;Não
						            ' as concluido
				        FROM		entidade.entidade ent
				        INNER JOIN  entidade.endereco ede on ede.entid = ent.entid
				        INNER JOIN  territorios.municipio mun on mun.muncod = ede.muncod
				        INNER JOIN 	proinfantil.procenso pro on pro.entcodent = ent.entcodent and pro.prcano = ({$_SESSION['exercicio']} - 1)
				        INNER JOIN 	proinfantil.mdssuplementacao sup on sup.entcodent = ent.entcodent and sup.prcid = pro.prcid AND sup.mdsstatus = 'A'        
						WHERE 		ent.entstatus = 'A'
						AND 		ent.tpcid in (1,2,3)
						AND 		ent.entcodent IS NOT NULL				
						AND			mun.muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'
									".(is_array($arWhere) ? ' AND '.implode(' AND ', $arWhere) : '')."
						ORDER BY 	ent.entnome";
				
				$db->monta_lista($sql, $cabecalho, 1000, 10, 'S', '', '', '', array(80, null, 120, 120, 120, 120,120,120, 120), array(null,null,'center','center','center','center','center','center')); ?>
			</td>		
		</tr>
		<tr>
			<td class="subtituloCentro" style="background-color: #CDCDCD;">Creches Conveniadas </td>		
		</tr>
		<tr>
			<td valign="top">
				<?php 
				
				if($entnome){
					$arWhere[] = " ent.entnome ilike '%{$entnome}%' ";	
				}
				
				if($entcodent){
					$arWhere[] = " ent.entcodent ilike '%{$entcodent}%' ";
				}
				
				$sql = "SELECT 		DISTINCT ent.entcodent||'&nbsp;' as entcodent,
				            		ent.entnome,
						            coalesce(pro.prcqtdalunoinfantilintegral,0) as prcqtdalunoinfantilintegral,
						            coalesce(mdsquantidadepbfintegral,0) as campo_integral,
						            coalesce(prcnummatcreche0a48integral, 0) as prcnummatcreche0a48integral,
						            coalesce(pro.prcqtdalunoinfantilparcial,0) as prcqtdalunoinfantilparcial,			            
						            coalesce(mdsquantidadepbfparcial,0) as campo_parcial,			            
						            coalesce(prcnummatcreche0a48parcial, 0) as prcnummatcreche0a48parcial,
						            '			            
						            <input type=\"radio\" name=\"mdsconferido_' || sup.mdsid || '\" value=\"t_' || sup.mdsid || '\" ' || case when mdsconferido = 't' then 'checked' else ' ' end || ' />&nbsp;Sim
						            <input type=\"radio\" name=\"mdsconferido_' || sup.mdsid || '\" value=\"f_' || sup.mdsid || '\" ' || case when mdsconferido = 'f' then 'checked' else ' ' end || ' />&nbsp;Não
						            ' as concluido
				        FROM        entidade.entidade ent
				        INNER JOIN  entidade.endereco ede on ede.entid = ent.entid
				        INNER JOIN  territorios.municipio mun on mun.muncod = ede.muncod
				        INNER JOIN 	proinfantil.procenso pro on pro.entcodent = ent.entcodent and pro.prcano = ({$_SESSION['exercicio']} - 1)
				        INNER JOIN 	proinfantil.mdssuplementacao sup on sup.entcodent = ent.entcodent and sup.prcid = pro.prcid AND sup.mdsstatus = 'A'        
						WHERE 		ent.entstatus = 'A'
						AND 		ent.tpcid in (4)
						AND 		ent.entcodent IS NOT NULL				
						AND			mun.muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'
									".(is_array($arWhere) ? ' AND '.implode(' AND ', $arWhere) : '')."
						ORDER BY 	ent.entnome";
				
				$db->monta_lista($sql, $cabecalho, 1000, 10, 'S', '', '', '', array(80, null, 120, 120, 120,120,120,120, 120), array(null,null,'center','center','center','center','center','center'));
				?>
			</td>
		</tr>
		<?php if($acesso['analise'] == 'S' || $_SESSION['data_hora_atual'] <= $_SESSION['data_hora_final']){ ?>
			<tr>
				<td>
					<center><input id="btnSalvar" type="button" value="Salvar" class="btnSalvar"></center>
				</td>
			</tr>
		<?php } ?>
	</table>
</form>
<br></br>
<?php 

$sql = "-- ESCOLAS PÚBLICAS 
        SELECT   	coalesce(sum( sup.mdsquantidadepbfparcial ),0) as mdsquantidadepbfparcial,  
        			coalesce(sum( sup.mdsquantidadepbfintegral ),0) as mdsquantidadepbfintegral
        FROM 		entidade.entidade ent
        INNER JOIN 	entidade.endereco ede on ede.entid = ent.entid
        INNER JOIN 	territorios.municipio mun on mun.muncod = ede.muncod
        INNER JOIN 	proinfantil.procenso pro on pro.entcodent = ent.entcodent and pro.prcano = ({$_SESSION['exercicio']} - 1)
        INNER JOIN 	proinfantil.mdssuplementacao sup on sup.entcodent = ent.entcodent and sup.prcid = pro.prcid AND sup.mdsstatus = 'A'
        WHERE      	mun.muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'
        AND        	ent.tpcid in (1,2,3)";

$totalMunicipal = $db->pegaLinha($sql);

$sql = "-- ESCOLAS PRIVADAS  
        SELECT     	coalesce(sum( sup.mdsquantidadepbfparcial ),0) as mdsquantidadepbfparcial,
        			coalesce(sum( sup.mdsquantidadepbfintegral ),0) as mdsquantidadepbfintegral
        FROM 		entidade.entidade ent
        INNER JOIN 	entidade.endereco ede on ede.entid = ent.entid
        INNER JOIN 	territorios.municipio mun on mun.muncod = ede.muncod
        INNER JOIN 	proinfantil.procenso pro on pro.entcodent = ent.entcodent and pro.prcano = ({$_SESSION['exercicio']} - 1)
        INNER JOIN 	proinfantil.mdssuplementacao sup on sup.entcodent = ent.entcodent and sup.prcid = pro.prcid AND sup.mdsstatus = 'A'
        WHERE       mun.muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'
		AND 		ent.tpcid in (4)";

$totalPrivada = $db->pegaLinha($sql);

?>
<?php if(!checkPerfil(array(EQUIPE_MUNICIPAL,SECRETARIO_ESTADUAL), false)): ?>
	
	<?php
	$arrReferencia = pegaValorReferencia();
	//$arrReferencia[TipoTurma][TipoAtendimentoTurma][TipoRede]
	if($_SESSION['exercicio'] == '2013'){ 
		$valor_creche_pub_int = $arrReferencia[1][1][1];
		$valor_creche_pub_par = $arrReferencia[1][2][1];
		$valor_creche_con_int = $arrReferencia[1][1][2];
		$valor_creche_con_par = $arrReferencia[1][2][2];
		
	} elseif($_SESSION['exercicio'] == '2012'){
		$valor_creche_pub_int = $arrReferencia[1][1][1];
		$valor_creche_pub_par = $arrReferencia[1][2][1];
		$valor_creche_con_int = $arrReferencia[1][1][2];
		$valor_creche_con_par = $arrReferencia[1][2][2];	
	}
	
	?>
	
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" >
		<tr>
			<td colspan="5" class="subtituloCentro" style="background: #dcdcdc"> Cálculo de Repasse para o Município </td>
		</tr>
		<tr>
			<td colspan="2" class="subtituloCentro" style="background: #dcdcdc"> Municipal: </td>		
			<td width="1" style="background: #dcdcdc" rowspan="5"></td>		
			<td colspan="2" class="subtituloCentro" style="background: #dcdcdc"> Privada/Conveniada: </td>
		</tr>
		<tr>
			<td class="subtituloDireita"> Quantidade Parcial: </td>
			<td class="subtituloDireita"> Valor de Repasse Parcial: </td>
			
			<td class="subtituloDireita"> Quantidade Parcial: </td>
			<td class="subtituloDireita"> Valor de Repasse Parcial: </td>		
		</tr>	
		<tr>
			<td align="right"> <?php echo $totalMunicipal['mdsquantidadepbfparcial']; ?> </td>
			<td align="right"> R$ <?php echo number_format($valor_creche_pub_par,2,',','.'); ?></td>
			
			<td align="right"> <?php echo $totalPrivada['mdsquantidadepbfparcial']; ?> </td>
			<td align="right"> R$ <?php echo number_format($valor_creche_con_par,2,',','.'); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita"> Quantidade Integral: </td>
			<td class="subtituloDireita"> Valor de Repasse Integral: </td>
				
			<td class="subtituloDireita"> Quantidade Integral: </td>
			<td class="subtituloDireita"> Valor de Repasse Integral: </td>		
		</tr>
		<tr>
			<td align="right"> <?php echo $totalMunicipal['mdsquantidadepbfintegral']; ?> </td>
			<td align="right"> R$ <?php echo number_format($valor_creche_pub_int,2,',','.'); ?></td>
			
			<td align="right"> <?php echo $totalPrivada['mdsquantidadepbfintegral']; ?> </td>
			<td align="right"> R$ <?php echo number_format($valor_creche_con_int,2,',','.'); ?></td>
		</tr>
		<tr>
			<td colspan="5" style="background: #dcdcdc" align="center"> 
				<b>Total:&nbsp;
				<?php 
				if($_SESSION['exercicio'] == '2013'){
					echo formata_valor(($totalMunicipal['mdsquantidadepbfparcial']*$valor_creche_pub_par)+($totalPrivada['mdsquantidadepbfparcial']*$valor_creche_con_par)+($totalMunicipal['mdsquantidadepbfintegral']*$valor_creche_pub_int)+($totalPrivada['mdsquantidadepbfintegral']*$valor_creche_con_int)); 
				} elseif($_SESSION['exercicio'] == '2012'){
					echo formata_valor(($totalMunicipal['mdsquantidadepbfparcial']*$valor_creche_pub_par)+($totalPrivada['mdsquantidadepbfparcial']*$valor_creche_con_par)+($totalMunicipal['mdsquantidadepbfintegral']*$valor_creche_pub_int)+($totalPrivada['mdsquantidadepbfintegral']*$valor_creche_con_int));
				} ?>
				</b> 
			</td>
		</tr>
	</table>
<?php endif; ?>

<?php if($docid && (!checkPerfil(array(EQUIPE_MUNICIPAL,SECRETARIO_ESTADUAL), false))): ?>
	<div style="position:absolute;top:270px;right:70px;">
		<?php		
		$oculta = true;
		if(true){
			$oculta = false;
		}			
		wf_desenhaBarraNavegacao( $docid , array('cpmid' => $cpmid),  array('historico'=>$oculta) );
		?>
	</div>
<?php endif; ?>

<script>
	$(function(){
		//.listagem
		$.each($('.listagem td'), function(i,v){
			if(v.title == 'Ordenar por Matrículas de 0 a 48 meses Parcial - Censo' || 
			   v.title == 'Ordenar por Matrículas de 0 a 48 meses Integral - Censo' || 
			   v.title == 'Matrículas de 0 a 48 meses Integral - Censo' || 
			   v.title == 'Matrículas de 0 a 48 meses Parcial - Censo'){
			   $(v).css('background', '#C3E0D8');	
			}
		})
	});

	<?php if(($acesso['analise'] == 'N' && $_SESSION['data_hora_atual'] >= $_SESSION['data_hora_final']) || $esdid != WF_MDS_EM_ANALISE) { ?>
		$('input, textarea').attr('disabled', true);
	<?php } ?>

	$('#btnVoltar').attr('disabled', false); 
	<?php if($acesso['analise'] == 'N'){ ?>
		$('#btnSalvar').hide();
		$('[name="tipid"]').attr('disabled', true);
		$('[name^="mdsconferido_"]').attr('disabled', true);
	<?php } ?>
</script>
