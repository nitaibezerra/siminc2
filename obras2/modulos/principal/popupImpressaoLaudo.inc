<?
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/EntidadeEndereco.php";

ini_set("memory_limit", "64M");
header('Content-type: text/html; charset="iso-8859-1"',true);

$sueid = $_REQUEST['sueid'];
$_SESSION['obras2']['sueid'] = $sueid;

if($_GET['empid']){
	$_SESSION['obras2']['empid'] = $_GET['empid'];
}

function pegaUfEmpresa($sosid){
    $sql = 'select mun.mundescricao, mun.estuf from entidade.entidade ent
        join entidade.endereco ed on ent.entid = ed.entid
        join territorios.municipio mun on mun.muncod = ed.muncod
        where ent.entid = (
            select e.entid from obras2.supervisao_os sos
            join obras2.supervisao_grupo_empresa sge on sos.sgeid = sge.sgeid
            join entidade.entidade e ON e.entid = sge.entid
            where sosid = '.$sosid.'
        )'; 
    return $sql;
}

//recuperando os dados da vistoria
$supervisaoEmpresa = new SupervisaoEmpresa( $sueid );
extract( $supervisaoEmpresa->getDados() );

$empreendimento = new Empreendimento( $_SESSION['obras2']['empid'] );


$sql = "SELECT
                sosid
        FROM
                obras2.supervisao_os_obra
        WHERE
                soostatus = 'A' AND
                empid = " . $_SESSION['obras2']['empid'];
$sosid = $db->pegaUm($sql);

$os 	= new Supervisao_Os( $sosid );
?>
<html>
	<head>
		<title>LAUDO TÉCNICO DE SUPERVISÃO DE OBRA</title>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
		<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
		<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
	</head>
	<body topmargin="0" leftmargin="0">
		<form method="post" action="" name="form_aditivo" id="form_aditivo" enctype="multipart/form-data">
			<center>
			<?php 
			echo monta_cabecalho_relatorio( '95' );
			?>
			</center>
			<table class="Tabela" align="center" cellPadding="3">
                                <tr>
			  		<td style="text-align: center; font-weight: bold;">
			  			Laudo de supervisão
			  		</td>
			  	</tr>
			  	<tr>
			  		<td>
			  			<div style="float: right;">
                                                    <div class="no-print">
                                                    <?php
                                                        $docid = pegaDocidSupervisaoEmpresa( $sueid );
                                                        // Barra de estado WORKFLOW
                                                        if($docid){
                                                            if( possui_perfil(PFLCOD_CALL_CENTER)){
                                                                wf_desenhaBarraNavegacao($docid, array('sueid' =>  $sueid), array('acoes' => true));
                                                            }else{
                                                                wf_desenhaBarraNavegacao($docid, array('sueid' =>  $sueid));
                                                            }
                                                        }
                                                    ?>
                                                    </div>
			  			</div>
			  			<p>
				  			<? 
                                                            $vistoriador 		= new Entidade( $entidvistoriador );
//                                                            $endereco               = new Endereco($entidvistoriador);
//                                                            
//                                                            $municipio              = new Municipio($endereco->muncod);
                                                            $sql = "
                                                                    select
                                                                            u.*
                                                                    from
                                                                            obras2.tecnico_empresa u
                                                                    where
                                                                            u.sgeid = ".$os->sgeid." and
                                                                            u.temcpf = '".formatar_cpf($vistoriador->entnumcpfcnpj)."'
                                                                    limit 1
                                                            ";
                                                            $usuarioTemp = $db->recuperar( $sql );
                                                        ?>
                                                        <?=$vistoriador->entnome;?>, Engenheiro Civil registrado no CREA sob nº <?=$usuarioTemp['temnumcreacau']?>, profissional da 
							<? 
                                                            $empresa = $empreendimento->buscaEmpresaOS();
							?>
							Empresa <?=$empresa['descricao'];?> CNPJ nº <?=$empresa['cnpj'] ?>, 
                                                        e conforme estabelecido em contrato firmado entre esta 
							empresa e o Fundo Nacional de Desenvolvimento da Educação, por meio do Pregão Eletrônico nº 
							05/2013, apresenta seu parecer  técnico conforme segue:
						</p>
						<p>
							<b>1 - Considerações preliminares</b> 
						</p>
						<p>
							Tem o presente a finalidade de certificar se na execução da obra em questão foram  
							cumpridas integralmente todas as condições estabelecidas no Projeto Básico apresentado/aprovado 
							e contrato firmado com a empreiteira.
						</p>
						<p>
							<b>2 - Características da OBRA.</b> 
						</p>
						<p>
							<? 
							if( $_SESSION['obras2']['obrid'] == '' ){
								
								$dados = $empreendimento->dadosResumo($_SESSION['obras2']['empid']);
								extract($dados);
							}else{
								$objeto = new Obras();
								$dados = $objeto->getDadosCabecalho($_SESSION['obras2']['obrid']);
                                extract($dados);
							}
							$obra = new Obras($empid);
                            $obra = $obra->pegaObraPorEmpid($empid);
							echo "({$obra['obrid']}) {$obra['obrnome']} localizado em $mundescricao - $estuf";
							?>
						</p>
						<p>
							<b>3 - Histórico  apresentado</b> 
						</p>
						<p>
							As informações levantadas quando da Supervisão estão lançadas no  <?php echo SIGLA_SISTEMA; ?> - Sistema 
							Integrado de Monitoramento, Execução e Controle do Ministério da Educação, no Módulo 
							Monitoramento de Obras, na Aba Vistoria, no campo  Relatório Técnico de Supervisão, conforme 
							Manual do Usuário do Sistema.
						</p>
						<p>
							<b>4 - Conclusões e Considerações finais:</b>
						</p>
						<p>
							Certificamos que a Obra de reforma/construção/ampliação encontra-se: 
							<br>
							<input type="radio" <?php echo ($supervisaoEmpresa->sobid == SOBID_EXECUCAO ? 'checked="checked"' : '') ?> disabled="disabled"> Em Execução 
							<input type="radio" <?php echo ($supervisaoEmpresa->sobid == SOBID_CONCLUIDA ? 'checked="checked"' : '') ?> disabled="disabled"> Concluída 
							<input type="radio" <?php echo ($supervisaoEmpresa->sobid == SOBID_PARALISADA ? 'checked="checked"' : '') ?> disabled="disabled"> Paralisada 
                                                        <input type="radio" <?php echo ($supervisaoEmpresa->sobid == 5 ? 'checked="checked"' : '') ?> disabled="disabled"> Não Iniciada 
							<br>
							<br>
                                                        <?php if($supervisaoEmpresa->sobid != 5){?>
							<input type="radio" <?php echo ($supervisaoEmpresa->sueacordo == 's' ? 'checked="checked"' : '') ?> disabled="disabled"> Em conformidade com o Projeto Básico aprovado e contrato.
							<br>
							<input type="radio" <?php echo ($supervisaoEmpresa->sueacordo == 'n' ? 'checked="checked"' : '') ?> disabled="disabled"> Em desacordo com o Projeto Básico aprovado e/ou contrato.
                                                        <?php }?>
						</p>
						<p>
							<b>5- Encerramento</b>
						</p>
						<p>
							Responsabilizo-me pelas informações lançadas no Sistema Integrado de Monitoramento, Execução 
							e Controle do Ministério da Educação decorrentes da supervisão realizada na obra identificada no 
							item 2. Este signatário apresenta o presente LAUDO TÉCNICO DE SUPERVISÃO DE OBRA, que 
							segue devidamente datado e assinado, colocando-se à  disposição para quaisquer esclarecimentos 
							adicionais que se fizerem necessários.
						</p>
						<p>
                                                        <?
                                                            $usuarioHomol = $os->pegaUltimaAtualizadaoSupervisao($sueid);
                                                            $sql = "
                                                                        select
                                                                                u.*
                                                                        from
                                                                                obras2.tecnico_empresa u
                                                                        where
                                                                                u.sgeid = ".$os->sgeid." and
                                                                                u.temcpf = '".formatar_cpf($usuarioHomol['usucpf'])."'
                                                                        limit 1
                                                                ";
                                                                $usuarioTemp = $db->recuperar( $sql );
                                                            extract( $db->pegaLinha(pegaUfEmpresa($sosid)) );
                                                        ?>
							<?=$mundescricao?>/<?=$estuf?> <?= $usuarioHomol['htddata'] ?><br>
							<?=$usuarioHomol['usunome'] ?>, nº do CREA <?=$usuarioTemp['temnumcreacau']?>.
						</p>
						<p>
							<b>
								VALIDAÇÃO DA  EMPRESA CONTRATADA, via <?php echo SIGLA_SISTEMA; ?>:
							</b>
                                                        <br />
                            <?
                                $usuarioHomol = $os->pegaUltimaAtualizadaoSupervisao($sueid, true);
                            ?>
                                                        <?=$usuarioHomol['usunome'] ?> - <?= $usuarioHomol['htddata'] ?>
						</p>	
			  		</td>
			  	</tr>
			  	<tr class="no-print" bgcolor="#C0C0C0">
			  		<td>
			  			<input type="button" class="noprint" value="Imprimir" onclick="window.print();">
			  			<input type="button" class="noprint" value="Fechar" onclick="window.close();">
			  		</td>
			  	</tr>
			</table>
		</form>	
        <style>
            @media print{    
                .no-print, .no-print *{
                    display: none !important;
                }
            }
        </style>
	</body>
</html>
