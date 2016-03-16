<?
include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';

monta_titulo( 'Lista de Pólos', ' ' );

// unset($_SESSION['projovemurbano']);

include_once APPRAIZ.'projovemurbano/modulos/principal/filtroAnoExercicio.inc';

$ano = '';
if( $_SESSION['projovemurbano']['ppuid'] == '2' ){
	$ano = '2013';
}elseif($_SESSION['projovemurbano']['ppuid'] == '3' ){
	$ano = '2014';
}
?>
<?php if($_SESSION['projovemurbano']['ppuano'] && $_SESSION['projovemurbano']['ppuid']){ ?>
	<script>
	function verMonitoramento(pjuid) {
		window.location='projovemurbano.php?modulo=principal/monitoramento<?=$ano?>&acao=A&pjuid='+pjuid;
	}
	</script>
	<?
	if(!$db->testa_superuser()) {
		
		$perfis = pegaPerfilGeral();
		
		if(in_array(PFL_DIRETOR_POLO, $perfis)) {
			$inner_polo = "INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND ur.polid=p.polid AND rpustatus='A'";
		}elseif(in_array(PFL_DIRETOR_NUCLEO, $perfis)) {
		
			$inner_nucleo = "INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND ur.entid = nes.entid AND rpustatus='A'";
			
		}elseif(in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)){
			$inner_escolas = "INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND ur.muncod = mu.muncod AND rpustatus='A'";
			$where = "AND pj.muncod is not null";
		}elseif(in_array(PFL_COORDENADOR_ESTADUAL, $perfis)){
			$inner_escolas = "	INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND ur.estuf = mu.estuf AND rpustatus='A'";
			$where = "AND pj.muncod is null";
		}
// 		elseif(in_array(PFL_SECRETARIO_MUNICIPAL, $perfis)){
// 			$inner_escolas = "INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND ur.muncod = mun.muncod AND rpustatus='A'";
// 		}elseif(in_array(PFL_SECRETARIO_ESTADUAL, $perfis)){
// 			$inner_escolas = "	INNER JOIN projovemurbano.projovemurbano pj ON pj.pjuid = pm.pjuid AND pj.muncod is null
// 								INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND ur.estuf = mu.estuf AND rpustatus='A'";
// 		}
	}
	if($_REQUEST['acao']=='P') {
	
		if($_SESSION['projovemurbano']['ppuid'] == 1){
			$ppu = "AND p.ppuid = {$_SESSION['projovemurbano']['ppuid']}";
		}
		$sql = "SELECT '<center><img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"verMonitoramento('||pm.pjuid||')\"></center>' as acao, 
					   CASE WHEN pj.muncod IS NOT NULL THEN 'Secretária Municipal - '||mu.mundescricao
						    WHEN pj.estuf  IS NOT NULL THEN 'Secretária Estadual - ' ||es.estdescricao END as descricao,
					   '<font size=1>PÓLO '||p.polid||', CEP:'||COALESCE(polcep::text,'Não informado')||' - End.:'||COALESCE(polendereco,'Não informado')||' - Bai.:'||COALESCE(polbairro,'Não informado')||'</font>' as polo,
					   (SELECT COUNT(*) as num FROM projovemurbano.cadastroestudante ce WHERE ce.polid=p.polid AND ce.ppuid = {$_SESSION['projovemurbano']['ppuid']} ) as qtdestudantes 
					FROM projovemurbano.polo p 
					INNER JOIN projovemurbano.polomunicipio pm on pm.pmuid = p.pmuid 
					INNER JOIN projovemurbano.projovemurbano pj on pj.pjuid = pm.pjuid 
					LEFT JOIN territorios.municipio mu on mu.muncod = pj.muncod 
					LEFT JOIN territorios.estado es on es.estuf = pj.estuf 
					{$inner_polo}
					$inner_escolas
					WHERE polstatus='A'
					$where 
					$ppu
					AND pj.ppuid = {$_SESSION['projovemurbano']['ppuid']}
					ORDER BY descricao";
// 		ver($sql,d);
		$cabecalho = array( "Ação", "Secretária", "Dados Pólo","Quantidade de estudantes");
		$db->monta_lista($sql,$cabecalho,50,10,'N','center','N',$nomeformulario="",$celWidth="",$celAlign="",$tempocache=null);
		
	}
	
	if($_REQUEST['acao']=='N') {
		if($_SESSION['projovemurbano']['ppuid'] == 1){
			$ppu = "AND p.ppuid = {$_SESSION['projovemurbano']['ppuid']}";
		}
		$sql = "(
				SELECT 
					'<center><img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"verMonitoramento('||pm.pjuid||')\">
					</center>' as acao, 
					mu.estuf||' - '||mu.mundescricao as localizacao, 
					'NÚCLEO '||nuc.nucid||CASE WHEN nes.nuetipo='S' THEN ', SEDE: ' ELSE ', ANEXO: ' END||ent.entnome as descricao,
					(SELECT COUNT(*) as num 
					FROM projovemurbano.cadastroestudante ce 
					INNER JOIN projovemurbano.turma tu ON tu.turid = ce.turid
					WHERE tu.entid = ent.entid AND ce.ppuid = nuc.ppuid) as qtdestudantes 
				FROM 
					projovemurbano.nucleo nuc
				INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
				INNER JOIN entidade.entidade ent ON ent.entid = nes.entid
				INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
				INNER JOIN territorios.municipio mu ON mu.muncod = mun.muncod 
				INNER JOIN projovemurbano.polomunicipio pm ON pm.pmuid = mun.pmuid 
				{$inner_nucleo}
				$inner_escolas
				WHERE 
					munstatus='A' 
					AND nuc.nucstatus='A' 
					and nuc.ppuid = {$_SESSION['projovemurbano']['ppuid']}
				ORDER BY 
					localizacao, nuc.nucid 
				) UNION ALL (
				SELECT 
					'<center><img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"verMonitoramento('||pm.pjuid||')\">
					</center>' as acao, 
					mu.estuf||' - '||mu.mundescricao as localizacao, 
					'NÚCLEO '||nuc.nucid||CASE WHEN nes.nuetipo='S' THEN ', SEDE: ' ELSE ', ANEXO: ' END||ent.entnome as descricao,
					(SELECT COUNT(*) as num 
					FROM projovemurbano.cadastroestudante ce 
					INNER JOIN projovemurbano.turma tu ON tu.turid = ce.turid
					WHERE tu.entid = ent.entid AND ce.ppuid = nuc.ppuid) as qtdestudante
				FROM 
					projovemurbano.nucleo nuc
				INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
				INNER JOIN entidade.entidade ent ON ent.entid = nes.entid
				INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
				INNER JOIN territorios.municipio mu ON mu.muncod = mun.muncod 
				INNER JOIN projovemurbano.associamucipiopolo asm ON asm.munid = mun.munid 
				INNER JOIN projovemurbano.polo pol ON pol.polid = asm.polid 
				INNER JOIN projovemurbano.polomunicipio pm ON pm.pmuid = pol.pmuid 
				{$inner_nucleo} 
				$inner_escolas 
				WHERE 
					mun.munstatus='A' 
					AND nuc.nucstatus='A' 
					and nuc.ppuid = {$_SESSION['projovemurbano']['ppuid']}
				ORDER BY 
					localizacao, nuc.nucid
				)";
// 	ver($sql,d);
		$cabecalho = array( "Ação", "Secretária", "Dados Núcleo","Quantidade de estudantes");
		$param['ordena'] = false;
		$db->monta_lista($sql,$cabecalho,50,10,'N','center','N',$nomeformulario="",$celWidth="",$celAlign="",$tempocache=null,$param);
		
	}
	
	?>
<?} ?>
