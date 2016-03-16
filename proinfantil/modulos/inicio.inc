<?php

include APPRAIZ . "includes/cabecalho.inc";
echo "<br />";
monta_titulo("E.I. Manutenção", "Selecione o módulo desejado:");

unset($_SESSION['proinfantil']);

$perfil = pegaperfil($_SESSION['usucpf']);

?>
<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
<style>
.box
{
	width: 30%;
	height: 80px;
	margin: 20px;
	float: left;
}

.box_titulo
{
	border: 1px solid rgb(136, 136, 136);
	width: 100%;
	background-color: rgb(238, 233, 233); 
	color: black; 
	font-weight: bold;
	text-align: center;
	padding-top: 5px;
	padding-bottom: 5px;
	font-size: 13px;
}

.box_corpo
{
	border-left: 1px solid rgb(136, 136, 136);
	border-right: 1px solid rgb(136, 136, 136);
	border-bottom: 1px solid rgb(136, 136, 136);
	cursor: pointer;
	width: 100%;
	height: 80px;
	text-align: center;
	background-color: #f5f5f5; 
	color: black;
	padding-top: 30px;
	padding-bottom: 30px;
	font-size: 13px;
}

.box_corpo1
{
	border-left: 1px solid rgb(136, 136, 136);
	border-right: 1px solid rgb(136, 136, 136);
	border-bottom: 1px solid rgb(136, 136, 136);
	width: 100%;
	height: 80px;
	text-align: center;
	background-color: #f5f5f5; 
	color: black;
	padding-top: 30px;
	padding-bottom: 30px;
	font-size: 13px;
}

.box_corpo:hover{
	background: #f0f0f0;
}

a
{
	cursor:pointer;
}
</style>

<?php $resolucao = verificaResolucao(); ?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="height:250px;">
	<tr>		
		<td style="text-align:center;" valign="top">
			<div class="box">
				<div class="box_titulo">Unidades do Proinfância</div>
				<div class="box_corpo">
					<div onclick="window.location.href='proinfantil.php?modulo=principal/listaObrasMunicipio&acao=A'">
						<a href="proinfantil.php?modulo=principal/listaObrasMunicipio&acao=A">
							Clique para acessar o módulo <br/>Unidades do Proinfância<br>&nbsp;
						</a>
					</div>
					<div>
						<a href="documentos/Resolucao_15_2013_Unidades_do_Proinfancia.pdf" target="_blank"><b><?php echo $resolucao['1']; ?></b></a>
					</div>					
				</div>
			</div>
			<?php if(in_array(PERFIL_SUPER_USUARIO, $perfil) || in_array(PERFIL_ADMINISTRADOR, $perfil) || in_array(PERFIL_ANALISTA, $perfil) || in_array(EQUIPE_MUNICIPAL, $perfil) || in_array(CONSULTA_GERAL, $perfil) ){ ?>
			<div class="box">
				<div class="box_titulo">Novas Turmas de Educação Infantil</div>
				<div class="box_corpo">
					<div onclick="window.location.href='proinfantil.php?modulo=novasturmas/listaMunicipios&acao=A'">
						<a href="proinfantil.php?modulo=novasturmas/listaMunicipios&acao=A">
							Clique para acessar o módulo <br/>Novas Turmas de Educação Infantil<br>&nbsp;
						</a>
					</div>
					<div>
						<a href="documentos/Resolucao_16_2013_Novas_turmas_EI.pdf" target="_blank"><b><?php echo $resolucao['2']; ?></b></a>
						<br>
						<!--  <b>Sistema em atualização de acordo com a nova resolução.</b>-->
					</div>
				</div>	
				<?php /*} else { ?>
				<div class="box_corpo1">
						<br>
						<b>Sistema em atualização de acordo com a nova resolução.</b>
				</div>
				<?php }*/ ?>					
			</div>
			<?php
			}
			$arrAnoCenso = $db->carregarColuna("SELECT (p.prcano + 1) as ano FROM proinfantil.procenso p GROUP BY p.prcano");
			if( in_array( $_SESSION['exercicio'], $arrAnoCenso ) ){ ?>
			<div class="box">
				<div class="box_titulo">Suplementação de Creches MDS</div>
				<div class="box_corpo">
					<div onclick="window.location.href='proinfantil.php?modulo=suplementacaomds/listaMunicipios&acao=A'">
						<a href="proinfantil.php?modulo=suplementacaomds/listaMunicipios&acao=A">
							Clique para acessar o módulo <br/>Suplementação de Creches MDS<br>&nbsp;
						</a>
					</div>	
					<div>
						<a href="documentos/Resolucao_17_2013_Suplementacao_de_Creches_MDS.pdf" target="_blank"><b><?php echo $resolucao['3']; ?></b></a>
						<br/>
						<?php 
						$_SESSION['data_hora_atual'] = strtotime(date('Y-m-d H:i'));
						$_SESSION['data_hora_final'] = strtotime($_SESSION['exercicio'].'-11-30 23:59');	
						if($_SESSION['data_hora_atual'] >= $_SESSION['data_hora_final']){ ?>
							<b><font style="color: blue;">Prazo de Preenchimento Encerrado</font></b>
						<?php }?>
					</div>
				</div>
			</div>
		<? } else { ?>
			<div class="box">
				<div class="box_titulo">Suplementação de Creches MDS</div>
				<div class="box_corpo" style="text-align: justify; height: 110px; padding-top: 2px;">
					<div onclick="window.location.href='http://www.dataescolabrasil.inep.gov.br/dataEscolaBrasil/home.seam'" style="padding: 5px">
						<a href="http://www.dataescolabrasil.inep.gov.br/dataEscolaBrasil/home.seam">
							A partir de 2014 as transferências de recursos da União aos Municípios e ao Distrito Federal,
							com a finalidade de prestar apoio financeiro suplementar à manutenção e ao
							desenvolvimento da educação infantil para atendimento em creches, serão realizadas, automaticamente, 
							pelo FNDE, com base na quantidade de matrículas de crianças de 0(zero) a 48(quarenta e oito) meses 
							cadastradas pelos Municípios e pelo Distrito Federal no Censo Escolar da Educação Básica 2013 cujas 
							famílias sejam beneficiárias do Programa Bolsa Família.
						</a>
					</div>	
				</div>
			</div>
		<?} ?>
		</td>
	</tr>
</table>