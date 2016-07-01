BEGIN TRANSACTION;
insert into fabrica.itemauditoria (itemnome, itemdsc, itemsituacao) VALUES
('Aderente ao template?', 'Aderente ao template?', true),
('Atualizado?', 'Artefato Atualizado?', true),
('Histórico atualizado?', 'Histórico atualizado?', true),
('Referencia ao repositório?', 'A SS faz referencia ao repositório correto?', true),
('Encontra-se no repositório?', 'Foi criado e se encontra no repositório?', true);
COMMIT;