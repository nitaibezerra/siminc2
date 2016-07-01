<?

$usucpf = $_SESSION['usucpf'];
$pflcod = pegaPerfil($usucpf);
$sql = "SELECT pflcod,usucpf,estuf,muncod,entid
		FROM pse.usuarioresponsabilidade
		WHERE 
		  usucpf = '$usucpf'
		  AND rpustatus='A'
		  AND pflcod = '$pflcod'";
$dados = $db->pegalinha($sql);

	if($db->testa_superuser()){
		$comboUFhab = "S";
		$comboMUNhab = "S";
	} 
	else {
		if($pflcod == SUPER_USUARIO || $pflcod == MEC || $pflcod == CONSULTA || $pflcod == PARCEIRO){
			$comboUFhab = "S";
			$comboMUNhab = "S";
	
		} 
		elseif ($pflcod == SECRETARIA_ESTADUAL){
			$uf = $dados['estuf'];
			if(!$uf){
				echo "<script>window.location.href='pse.php?modulo=principal/avisoPerfil&acao=A';</script>";
				exit;
			}
			else {
				$_SESSION['pse']['uf'] = $uf;
				$uf = $_SESSION['pse']['uf'];
				$comboUFhab = "N";
				$comboMUNhab = "S";
				$where.=" AND m.estuf='".$uf."'";
			}
		} 
		elseif ($pflcod == SECRETARIA_MUNICIPAL){
			$mun = $dados['muncod'];
			if(!$mun){
				echo "<script>window.location.href='pse.php?modulo=principal/avisoPerfil&acao=A';</script>";
				exit;
			}else {
				$_SESSION['pse']['muncod'] = $mun;
				$muncod = $_SESSION['pse']['muncod'];
				$comboUFhab = "N";
				$comboMUNhab = "N";
				$where.=" AND m.muncod='".$muncod."'";
			}
		}
		elseif ($pflcod == ESCOLA_MUNICIPAL || $pflcod == ESCOLA_ESTADUAL || $pflcod == DIRETOR_ESCOLA || $pflcod == EDUCADOR_ESCOLA){
			$ent = $dados['entid'];
			if(!$ent){
				echo "<script>window.location.href='pse.php?modulo=principal/avisoPerfil&acao=A';</script>";
				exit;
			}
			else {
				$_SESSION['pse']['entid'] = $ent;
				$entid = $_SESSION['pse']['entid'];
				$comboUFhab = "N";
				$comboMUNhab = "N";
				$btHab = "disabled";
				
				//verifica se possui mais de um entid
				$sql = "SELECT entid
						FROM pse.usuarioresponsabilidade
						WHERE 
						  usucpf = '$usucpf'
						  AND rpustatus='A'
						  AND pflcod = '$pflcod'";
				$arrayEntid = $db->carregarColuna($sql);
				if($arrayEntid){
					$where.=" AND ent.entid in(".implode(',',$arrayEntid).")";
				}
				
				$sql = "SELECT muncod FROM entidade.endereco WHERE entid = '{$_SESSION['pse']['entid']}'";
				$_SESSION['pse']['muncod'] = $db->pegaUm($sql);
				
			}
		}
		else {
			echo "<script>window.location.href='pse.php?modulo=principal/avisoPerfil&acao=A';</script>";
		}
	}



?>
