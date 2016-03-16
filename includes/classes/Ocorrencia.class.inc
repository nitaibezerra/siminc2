<?php
class Ocorrencia{
	private $ocoid, $post, $bojQueryUI, $right, $top;
	
	public function __construct( $arrParam = array() ){
		
		$this->ocoid 		= $arrParam['ocoid'];
		$this->post 		= $arrParam['post'];
		$this->bojQueryUI 	= (!empty($arrParam['bojQueryUI']) 	? $arrParam['bojQueryUI'] 	: 'N');
		$this->right 		= (!empty($arrParam['right']) 		? $arrParam['right'] 		: '40px');
		$this->top			= (!empty($arrParam['top'])			? $arrParam['top'] 			: '180px');
	}
	
	public function criaMenuIcon(){
		
		?>
		<style>
			#menuocorrencia * {margin: 0; padding: 0;}
			
			#menuocorrencia {
				width: 100px;
				margin: 0px auto 0 auto;
				color: white;
				position: absolute;
				/*border: solid 1px black;*/
				right: 25px;
				top: 20px;
				z-index: 100;
			}			
			#ocorrencia_main {
				position: absolute;
				/*border: solid 1px black;*/
				right: <?php echo $this->right; ?>;
				top: <?php echo $this->top; ?>;
				z-index: 105;
			}			
			.menuocorrenciaclick {
				background: #E3E3E3;
				border-radius: 5px;
				box-shadow: 
					0 1px 1px 1px rgba(0, 0, 0, 0.6);
			}
			/*heading styles*/
			#menuocorrencia h3 {
				font-size: 12px;
				line-height: 34px;
				padding: 0 0px;
				margin-top: 0;
				cursor: pointer;
				position: absolute;
				right: 98;
				top: -20px;
				/*fallback for browsers not supporting gradients*/
				/*background: #003040; 
				background: linear-gradient(#003040, #002535);*/
			}
			/*heading hover effect*/
			#menuocorrencia h3:hover {
				text-shadow: 0 0 1px rgba(255, 255, 255, 0.7);
			}
			/*iconfont styles*/
			#menuocorrencia h3 span {
				font-size: 16px;
				margin-right: 10px;
			}
			/*list items*/
			#menuocorrencia li {
				list-style-type: none;
			}
			/*links*/
			#menuocorrencia ul ul li a {
				color: black;
				text-decoration: none;
				font-size: 11px;
				line-height: 27px;
				display: block;
				padding: 0 15px;
				/*transition for smooth hover animation*/
				transition: all 0.15s;
			}
			/*hover effect on links*/
			#menuocorrencia ul ul li a:hover {
				background: #E9E9E9;
				border-left: 5px solid ;
				border-radius: 5px;
			}
			/*Lets hide the non active LIs by default*/
			#menuocorrencia ul ul {
				display: none;
			}
			#menuocorrencia li.active ul {
				display: block;
			}
		</style>
		<div id="ocorrencia_main" style="width: 140px; height: 100px;">
			<div id="menuocorrencia">
				<ul class="opMenu">
					<li>
						<h3><img alt="" src="../imagens/simular_usuario.png" title="Simular Usuário" style="width: 18px; cursor: pointer;"></h3>
						<ul>
							<li><a href="#" onclick="javascript:listaOcorrencias();">Lista</a></li>
							<li><a href="#" onclick="javascript:cadastraOcorrencias();">Cadastra</a></li>
						</ul>
					</li>
				</ul>
			</div>
		</div>
<?php 	if( $this->bojQueryUI == 'S' ){ ?>
			<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
			<link rel="stylesheet" type="text/css" href="../library/jquery/jquery-ui-1.10.3/themes/custom-theme/jquery-ui-1.10.3.custom.min.css"/>
<?php 	}	?>
		<script type="text/javascript">
		//jQuery.noConflict();
		jQuery(document).ready(function() {
			jQuery("#menuocorrencia h3").click(function(){
				
				jQuery("#menuocorrencia ul li ul li").addClass('menuocorrenciaclick');
				jQuery("#menuocorrencia ul ul").slideUp();
							
				if(!jQuery(this).next().is(":visible")){
					jQuery(this).next().slideDown();
				}
			});

			jQuery("#ocorrencia_main").mouseleave(function(){
				jQuery("#menuocorrencia ul ul").slideUp();
			});
		});
		</script>
		<?php
		$this->criaDivOcorrencia();
		$this->criaDivListaOcorrencia();
	}
	
	public function criaIcon(){
		?>
		<style>
			#menuocorrencia * {margin: 0; padding: 0;}

			#menuocorrencia {
				width: 100px;
				margin: 0px auto 0 auto;
				color: white;
				position: absolute;
				right: 20;
				top: 180px;
				z-index: 100;
			}
		</style>
		<div id="menuocorrencia">
			<img alt="" src="../imagens/simular_usuario.png" title="Simular Usuário" style="width: 18px; cursor: pointer;" onclick="javascript:cadastraOcorrencias();">
		</div>
<?php 	if( $this->bojQueryUI == 'S' ){ ?>
			<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
			<link rel="stylesheet" type="text/css" href="../library/jquery/jquery-ui-1.10.3/themes/custom-theme/jquery-ui-1.10.3.custom.min.css"/>
<?php 	}	?>
		<?php
		$this->criaDivOcorrencia();
		$this->criaDivListaOcorrencia();
	}
	
	public function criaDivOcorrencia(){
		global $db;
		
		if( $this->bojQueryUI == 'N' ){
			$tamCombo = 448;
			$tamText = 59;
		} else {
			$tamCombo = 505;
			$tamText = 58;
		}
		?>
		<div id="dlg_ocorrencia_confirm" title="Ocorrências" ></div>
		<div id="dlg_ocorrencia" title="Cadastrar Ocorrências" style="display: none;">
		<form name="formOcorrencias" id="formOcorrencias" method="post" action="" >
			
			<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
				<tr>
					<td class="subtituloDireita" width="30%">Área Responsável pela Resolução:</td>
					<td><?php 
					$sql = "SELECT 
							  	arnid as codigo,
							  	arndescricao as descricao					  	
							FROM 
							  	seguranca.areanegocio
							WHERE sisid = {$_SESSION['sisid']} and arnstatus = 'A'";
					$db->monta_combo( "arnid", $sql, 'S', 'Selecione...', '', '', '', $tamCombo ); ?></td>
				</tr>
				<tr>
					<td class="subtituloDireita" width="30%">Tipos de Ocorrências:</td>
					<td><?php 
					$sql = "SELECT 
							  	tioid as codigo,
							  	tiodescricao as descricao					  	
							FROM 
							  	seguranca.tipoocorrencia
							WHERE tiostatus = 'A' and sisid = {$_SESSION['sisid']}";
					$db->monta_combo( "tioid", $sql, 'S', 'Selecione...', '', '', '', $tamCombo ); ?></td>
				</tr>
				<tr>
					<td class="subtituloDireita" width="30%">Descrição:</td>
					<td><?php echo campo_texto('ocodescricao', 'N', 'S', 'Descrição', $tamText, 70, '', '', '', '', '', 'id="ocodescricao"', ''); ?></td>
				</tr>
				<tr>
					<td class="subtituloDireita" width="30%">Detalhe:</td>
					<td><?php echo campo_textarea('ocodetalhe', 'N', 'S', 'Detalhe', 59, 5, 500, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Descrição detalhada', '', array('id' => 'ocodetalhe')); ?>
				</tr>
			</table>
		</form>
		</div>
		<?php
	}
	
	public function criaDivListaOcorrencia(){
		global $db;
		$sql = "SELECT 
					oc.ocoid,
					an.arndescricao,
				  	tp.tiodescricao,
				  	oc.ocodescricao,
				  	oc.ocodetalhe,
				  	to_char(oc.ocodatainclusao, 'DD/MM/YYYY HH24:MI:SS') as data
				FROM 
				  	seguranca.ocorrencia oc
				    inner join seguranca.tipoocorrencia tp on tp.tioid = oc.tioid and tp.tiostatus = 'A' and tp.sisid = oc.sisid
				    inner join seguranca.areanegocio an on an.arnid = oc.arnid and an.arnstatus = 'A' and an.sisid = oc.sisid
				WHERE
					oc.ocostatus = 'A'
				    and oc.sisid = {$_SESSION['sisid']}
				    and oc.usucpf = '{$_SESSION['usucpf']}'";
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		$cabecalho = array('', 'Área de Negocio', 'Tipo de Ocorrência', 'Descrição', 'Detalhe', 'Data');
		$style = 'style="font-size: 12px !important; font-family: Verdana,Arial,sans-serif !important;"';
		?>
		<div id="dlg_lista_ocorrencia" title="Lista de Ocorrências" style="display: none;">			
<?php 	if( $arrDados ){ ?>
			<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" style="color:333333;">
			<thead>
				<tr>
					<td <?php echo $style;?> align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Abrir Link</b></td>
					<td <?php echo $style;?> align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Área de Negocio</b></td>
					<td <?php echo $style;?> align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Tipo de Ocorrência</b></td>
					<td <?php echo $style;?> align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Descrição</b></td>
					<td <?php echo $style;?> align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Detalhe</b></td>
					<td <?php echo $style;?> align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Data</b></td>
				</tr>
			</thead>
			<tbody>
<?php 		foreach ($arrDados as $key => $v) {
				$key % 2 ? $cor = "#dedfde" : $cor = ""; ?>
				<tr bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
					<td <?php echo $style;?> align="right" valign="top" style="color:#999999;" title=""><center>
						<img src="../imagens/preview.gif" style="cursor:pointer;" onclick="abrirLinkOcorrencia(<?php echo $v['ocoid'];?>);" ></center></td>
					<td <?php echo $style;?> valign="top" title="Área de Negocio"><?php echo $v['arndescricao']; ?></td>
					<td <?php echo $style;?> valign="top" title="Tipo de Ocorrência"><?php echo $v['tiodescricao']; ?></td>
					<td <?php echo $style;?> valign="top" title="Descrição"><?php echo $v['ocodescricao']; ?></td>
					<td <?php echo $style;?> valign="top" title="Detalhe"><?php echo $v['ocodetalhe']; ?></td>
					<td <?php echo $style;?> align="right" valign="top" style="color:#999999;" title=""><?php echo $v['data']; ?></td>
				</tr>
<?php 		}?>
			</tbody>
			</table>
			<table width="100%" align="center" border="0" cellspacing="0"
				cellpadding="2" class="listagem">
				<tr bgcolor="#ffffff">
					<td <?php echo $style;?>>
						<b>Total de Registros: <?php echo sizeof($arrDados);?></b>
					</td>
					<tr>
			</table>
<?php 	}else{
			print '<table width="100%" align="left" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			print '<tr><td align="center" style="color:#cc0000;">Não Foram Encontrados Registros de Ocorrências.</td></tr>';
			print '</table>';
		}?>
		</div>
		<?php
	}
	
	public function salvar(){
		global $db;
		
		$session = $_SESSION;
		
		unset($session['acl']);
		unset($session['sistemas']);
		unset($session['perfilusuario']);
		unset($session['perfils']);
		unset($session['perfil']);
		unset($session['menu']);
		unset($session['manuais']);
		unset($session['dadosusuario']);
		unset($session['rastro']);
		unset($session['tipodocumentos']);
		unset($session['indice_sessao_combo_popup']);
		unset($session['sql']);
		unset($session['abamenu']);
				
		$sql = "INSERT INTO seguranca.ocorrencia(arnid, tioid, sisid, usucpf, ocodescricao, ocodetalhe, ocotipotela, ocolink, ocosession, ocodatainclusao) 
				VALUES (
				  {$this->post['arnid']},
				  {$this->post['tioid']},
				  {$_SESSION['sisid']},
				  '{$_SESSION['usucpf']}',
				  '".utf8_decode( $this->post['ocodescricao'] )."',
				  '".utf8_decode( $this->post['ocodetalhe'] )."',
				  '{$this->post['tipotela']}',
				  '".$_SERVER['HTTP_REFERER']."',
				  '".json_encode( $session )."',
				  now()
				)";
		$db->executar($sql);
		if( $db->commit() ){
			echo 'Operação Realizada com Sucesso.';
		} else {
			echo 'Falha na Operação.';
		}
	}
}