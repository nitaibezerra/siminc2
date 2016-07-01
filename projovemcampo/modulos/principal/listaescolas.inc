<?
include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';

monta_titulo( 'Lista de Escolas', ' ' );

// unset($_SESSION['projovemcampo']);
?>
	<script>
	function verMonitoramento(apcid) {
		window.location='projovemcampo.php?modulo=principal/monitoramento&acao=A&apcid='+apcid;
	}
	</script>
	<?
	if(!$db->testa_superuser()) {
		
		$perfis = pegaPerfilGeral();
		
		if(in_array(PFL_DIRETOR_ESCOLA, $perfis)) {
		
			$inner_escolas = "INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND ur.entid = tur.entid AND rpustatus='A'";
			
		}
		if(in_array(PFL_SECRETARIO_ESTADUAL, $perfis)||in_array(PFL_COORDENADOR_ESTADUAL, $perfis)){
			$inner_escolas = "INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND ur.estuf = apc.apccodibge::character(2) AND rpustatus='A'";
		}
		if(in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)||in_array(PFL_SECRETARIO_MUNICIPAL, $perfis)){
			$inner_escolas = "INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND ur.muncod = apc.apccodibge::character(7) AND rpustatus='A'";
		}
		if(in_array(PFL_COORDENADOR_TURMA,$perfis)){
			$inner_escolas = "INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND ur.turid = tur.turid AND rpustatus='A'";
		}
	}
		$sql = "
				SELECT DISTINCT
					'<center><img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"verMonitoramento('||apc.apcid||')\">
					</center>' as acao, 
					mu.estuf||' - '||mu.mundescricao as localizacao, 
					ent.entnome as descricao,
					(SELECT COUNT(*) as num 
					FROM projovemcampo.estudante est 
					INNER JOIN projovemcampo.turma tu ON tu.turid = est.turid
					WHERE tu.entid = ent.entid ) as qtdestudantes 
				FROM 
					projovemcampo.turma tur
				INNER JOIN projovemcampo.adesaoprojovemcampo apc ON apc.secaid = tur.secaid
				INNER JOIN entidade.endereco ende ON ende.entid = tur.entid
				INNER JOIN entidade.entidade ent ON ent.entid = tur.entid
				INNER JOIN territorios.municipio mu ON mu.muncod = ende.muncod 
				$inner_escolas
				WHERE
					tur.turstatus = 'A'
				ORDER BY 
					localizacao 
								";
// 	ver($sql);
		$cabecalho = array( "Ação", "Secretária", "Dados Núcleo","Quantidade de estudantes");
		$param['ordena'] = false;
		$db->monta_lista($sql,$cabecalho,50,10,'N','center','N',$nomeformulario="",$celWidth="",$celAlign="",$tempocache=null,$param);
		
	
	?>
